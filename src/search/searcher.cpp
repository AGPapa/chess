#include <thread>
#include <sstream>

#include "search_job.cpp"

class Searcher {

    public:

        Searcher(std::ostream *output) {
            _root = std::unique_ptr<RootNode>(new RootNode(Board::default_board()));
            _state = SearcherState::Stopped;
            _output = output;
            _expand_queue = std::unique_ptr<MPSCQueue<ExpandJob>>(new MPSCQueue<ExpandJob>());
            _evaluate_queue = std::unique_ptr<SPMCQueue<EvaluateJob>>(new SPMCQueue<EvaluateJob>());
        }

        Searcher(Board starting_board, std::ostream *output) {
            _root = std::unique_ptr<RootNode>(new RootNode(starting_board));
            _state = SearcherState::Stopped;
            _output = output;
            _expand_queue = std::unique_ptr<MPSCQueue<ExpandJob>>(new MPSCQueue<ExpandJob>());
            _evaluate_queue = std::unique_ptr<SPMCQueue<EvaluateJob>>(new SPMCQueue<EvaluateJob>());
        }

        Ply find_best_ply() {
            Edge* best_child = nullptr;
            int most_visits = 0;
            for (int i = 0; i < _root->_num_children; i++) {
                if (_root->_children[i].visits() > most_visits) {
                    most_visits = _root->_children[i].visits();
                    best_child = &_root->_children[i];
                }
            }
            return best_child->_ply;
        }
/*
        void node_census(Node* root, int* leaves, int* expanded, int* terminal) {
            bool has_children = false;
            for (Node* child : root->children()) {
                has_children = true;
                if (child->is_leaf()){
                    (*leaves)++;
                } else {
                    (*expanded)++;
                    node_census((ExpandedNode*) child, leaves, expanded, terminal);
                }
            }
            if(!has_children) { (*terminal)++; }
        }*/

        void apply_ply(Ply p) {
            for (int i = 0; i < _root->_num_children; i++) {
                Edge* child = &_root->_children[i];
                if (child->_ply == p) {
                    if (child->is_leaf()) {
                        Board new_board = Board(_root->_board);
                        new_board.apply_ply(p);
                        _root = std::unique_ptr<RootNode>(new RootNode(new_board));
                        return;
                    } else {
                        _root = std::unique_ptr<RootNode>(new RootNode(_root->_board, child->_ply, child->_node.get()));
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
        enum class SearcherState : std::uint8_t { Stopped, StoppingSearch, StoppingEvaluation, Searching };

        std::unique_ptr<RootNode> _root;
        SearcherState _state;
        std::mutex _state_mutex;
        std::unique_ptr<std::thread> _searching_thread;
        std::unique_ptr<std::thread> _time_managing_thread;
        std::unique_ptr<std::thread> _evaluating_threads[NUM_EVALUATION_THREADS];
        std::condition_variable _time_managing_variable;
        std::condition_variable _search_variable;
        std::ostream *_output;
        std::unique_ptr<MPSCQueue<ExpandJob>> _expand_queue;
        std::unique_ptr<SPMCQueue<EvaluateJob>> _evaluate_queue;
        std::set<Edge*> _active_nodes;
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
                if (!_evaluate_queue->empty()) { throw std::runtime_error("Evaluate queue is not empty"); }
                if (!_expand_queue->empty()) { throw std::runtime_error("Expand queue is not empty"); }
                if (!_active_nodes.empty()) { throw std::runtime_error("Active nodes set is not empty"); }
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

            int leaves = 0;
            int expanded = 0;
            int terminal = 0;

            //node_census(_root.get(), &leaves, &expanded, &terminal);

            output << "info leaves " << leaves << std::endl;
            output << "info expanded " << expanded << std::endl;
            output << "info terminal " << terminal << std::endl;
        }

        void _search() {
            std::mutex search_mutex;
            std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(search_mutex);
            while (_state == SearcherState::Searching || !_expand_queue->empty()) {
                if (_state == SearcherState::Searching) {
                    SearchJob(_root.get()).run(&_active_nodes, _evaluate_queue.get());
                }
                _expand();
                if (_evaluate_queue->size() > (10 * NUM_EVALUATION_THREADS)) {
                    _search_variable.wait(lock);
                }
            }
        }

        void _evaluate() {
            EvaluateJob job;
            while (_state == SearcherState::Searching || _state == SearcherState::StoppingSearch || !_evaluate_queue->empty()) {
                while (!_evaluate_queue->empty()) {
                    bool success = _evaluate_queue->dequeue(&job);
                    if (success) {
                        job.run(_expand_queue.get());
                    }
                }
                _search_variable.notify_one();
            }
        }

        void _expand() {
            ExpandJob job;
            while (!_expand_queue->empty()) {
                bool success = _expand_queue->dequeue(&job);
                if (success) {
                    job.run(&_active_nodes);
                } else {
                    throw std::runtime_error("Dequeued nullptr expand job");
                }
            }
        }
};
