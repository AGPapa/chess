#include <thread>
#include <sstream>

#include "search_job.cpp"

class Searcher {

    public:

        Searcher(std::ostream *output) {
            _root = std::unique_ptr<RootNode>(new RootNode(Board::default_board()));
            _state = SearcherState::Stopped;
            _output = output;
            _backprop_queue = std::unique_ptr<MPSCQueue<BackpropJob>>(new MPSCQueue<BackpropJob>());
            _expand_queue = std::unique_ptr<MPSCQueue<ExpandJob>>(new MPSCQueue<ExpandJob>());
            _evaluate_queue = std::unique_ptr<MPSCQueue<EvaluateJob>>(new MPSCQueue<EvaluateJob>());
        }

        Searcher(Board starting_board, std::ostream *output) {
            _root = std::unique_ptr<RootNode>(new RootNode(starting_board));
            _state = SearcherState::Stopped;
            _output = output;
            _backprop_queue = std::unique_ptr<MPSCQueue<BackpropJob>>(new MPSCQueue<BackpropJob>());
            _expand_queue = std::unique_ptr<MPSCQueue<ExpandJob>>(new MPSCQueue<ExpandJob>());
            _evaluate_queue = std::unique_ptr<MPSCQueue<EvaluateJob>>(new MPSCQueue<EvaluateJob>());
        }

        Ply find_best_ply() {
            Node* best_child = nullptr;
            int most_visits = 0;
            for (Node* child : _root->children()) {
                if (child->visits() > most_visits) {
                    most_visits = child->visits();
                    best_child = child;
                }
            }
            return best_child->_ply;
        }

        void apply_ply(Ply p) {
            for (Node* child : _root->children()) {
                if (child->_ply == p) {
                    if (child->is_leaf()) {
                        Board new_board = Board(_root->_board);
                        new_board.apply_ply(p);
                        _root = std::unique_ptr<RootNode>(new RootNode(new_board));
                        return;
                    } else {
                        _root = std::unique_ptr<RootNode>(new RootNode(_root->_board, ((ExpandedNode*) child)));
                        return;
                    }
                }
            }
            throw std::runtime_error("Ply not found in tree");
        }

        void start_searching() {
            _state_mutex.lock();
            if (_state != SearcherState::Stopped) {
                throw std::runtime_error("Starting search while search is still running");
            }
            if (_time_managing_thread != nullptr && _time_managing_thread->joinable()) {
                _time_managing_thread->join();
            }
            _state = SearcherState::Searching;
            _searching_thread = std::unique_ptr<std::thread>(new std::thread(&Searcher::_search, this));
            for (int i = 0; i < NUM_EVALUATION_THREADS; i++) {
                _evaluating_threads[i] = std::unique_ptr<std::thread>(new std::thread(&Searcher::_evaluate, this));
            }
            _backpropagating_thread = std::unique_ptr<std::thread>(new std::thread(&Searcher::_backpropagate, this));
            _time_managing_thread = std::unique_ptr<std::thread>(new std::thread(&Searcher::_manage_time, this));
            _state_mutex.unlock();
        }

        void stop_searching() {
            if (_state == SearcherState::Searching) {
                _time_managing_variable.notify_one();
                _time_managing_thread->join();
            } else {
                if (_time_managing_thread != nullptr && _time_managing_thread->joinable()) {
                   _time_managing_thread->join();
                }
            }
        }

    private:
        enum class SearcherState : std::uint8_t { Stopped, StoppingSearch, StoppingEvaluation, StoppingBackprop, Searching };

        std::unique_ptr<RootNode> _root;
        SearcherState _state;
        std::mutex _state_mutex;
        std::unique_ptr<std::thread> _searching_thread;
        std::unique_ptr<std::thread> _backpropagating_thread;
        std::unique_ptr<std::thread> _time_managing_thread;
        std::unique_ptr<std::thread> _evaluating_threads[NUM_EVALUATION_THREADS];
        std::condition_variable _time_managing_variable;
        std::condition_variable _search_variable;
        std::condition_variable _backprop_variable;
        std::ostream *_output;
        std::unique_ptr<MPSCQueue<BackpropJob>> _backprop_queue;
        std::unique_ptr<MPSCQueue<ExpandJob>> _expand_queue;
        std::unique_ptr<MPSCQueue<EvaluateJob>> _evaluate_queue;
        std::set<LeafNode*> _active_nodes;
        int _w_time;
        int _b_time;
        int _moves_to_next_time_control;
        int _w_inc;
        int _b_inc;

        void _manage_time() {
            std::mutex time_managing_mutex;
            std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(time_managing_mutex);
            while (_state == SearcherState::Searching) {
                _time_managing_variable.wait_for(lock, std::chrono::milliseconds(10000));
                if (_state == SearcherState::Searching) {
                    _stop_searching();
                }
            }
        }

        void _stop_searching() {
            _state_mutex.lock();
            if (_state == SearcherState::Searching) {
                _state = SearcherState::StoppingSearch;
                _searching_thread->join();
                _state = SearcherState::StoppingEvaluation;
                for (int i = 0; i < NUM_EVALUATION_THREADS; i++) {
                    if (_evaluating_threads[i] != nullptr && _evaluating_threads[i]->joinable()) {
                        _evaluating_threads[i]->join();
                    }
                }
                _expand();
                _state = SearcherState::StoppingBackprop;
                _backprop_variable.notify_one();
                if (_backpropagating_thread != nullptr && _backpropagating_thread->joinable()) {
                   _backpropagating_thread->join();
                }
                if (!_evaluate_queue->empty()) { throw std::runtime_error("Evaluate queue is not empty"); }
                if (!_expand_queue->empty()) { throw std::runtime_error("Expand queue is not empty"); }
                if (!_backprop_queue->empty()) { throw std::runtime_error("Backprop queue is not empty"); }
                _state = SearcherState::Stopped;
                _output_info();
                _output_bestmove();
            }
            _state_mutex.unlock();
        }

        void _output_bestmove() {
            std::ostream& output = *_output;
            Ply best_ply = find_best_ply();
            output << "bestmove " << best_ply.to_string() << std::endl;
        }

        void _output_info() {
            std::ostream& output = *_output;
            int nodes = _root->visits();
            output << "info nodes " << nodes << std::endl;
        }

        void _search() {
            std::mutex search_mutex;
            std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(search_mutex);
            while (_state == SearcherState::Searching || !_expand_queue->empty()) {
                if (_state == SearcherState::Searching) {
                    SearchJob(_root.get()).run(&_active_nodes, _evaluate_queue.get(), _backprop_queue.get(), &_backprop_variable);
                }
                _expand();
                if (_evaluate_queue->size() > 50) { //TODO: investigate why we're not hitting this
                    _search_variable.wait(lock);
                }
            }
        }

        void _evaluate() {
            while (_state == SearcherState::Searching || _state == SearcherState::StoppingSearch || !_evaluate_queue->empty()) {
                while (!_evaluate_queue->empty()) {
                    std::unique_ptr<EvaluateJob> job = std::move(_evaluate_queue->dequeue());
                    if (job != nullptr) { // TODO: Find out why we need this - getting null jobs somehow??
                        job->run(_expand_queue.get());
                    }
                }
                _search_variable.notify_one();
            }
        }

        void _expand() {
            while (!_expand_queue->empty()) {
                std::unique_ptr<ExpandJob> job = std::move(_expand_queue->dequeue());
                if (job != nullptr) { // TODO: Find out why we need this - getting null jobs somehow??
                    job->run(&_active_nodes, _backprop_queue.get(), &_backprop_variable);
                }
            }
        }

        void _backpropagate() {
            std::mutex backprop_mutex;
            std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(backprop_mutex);
            while (_state == SearcherState::Searching || _state == SearcherState::StoppingSearch || _state == SearcherState::StoppingEvaluation || !_backprop_queue->empty()) {
                _backprop_variable.wait_for(lock, std::chrono::milliseconds(50));  //timeout here in case we miss the closing backprop_variable.notify() in _stop_searching
                while (!_backprop_queue->empty()) {
                    std::unique_ptr<BackpropJob> job = std::move(_backprop_queue->dequeue());
                    if (job != nullptr) { // TODO: Find out why we need this - getting null jobs somehow??
                        job->run();
                    }
                }
            }
        }
};
