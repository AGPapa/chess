#include <thread>
#include <sstream>

#include "root_node.cpp"
#include "leaf_node.cpp"
#include "mpsc_queue.cpp"
#include "backprop_job.cpp"

class Searcher {

    public:

        Searcher(std::ostream *output) {
            _root = std::unique_ptr<RootNode>(new RootNode(Board::default_board()));
            _state = SearcherState::Stopped;
            _output = output;
            _backprop_queue = std::unique_ptr<MPSCQueue<BackpropJob>>(new MPSCQueue<BackpropJob>());
        }

        Searcher(Board starting_board, std::ostream *output) {
            _root = std::unique_ptr<RootNode>(new RootNode(starting_board));
            _state = SearcherState::Stopped;
            _output = output;
            _backprop_queue = std::unique_ptr<MPSCQueue<BackpropJob>>(new MPSCQueue<BackpropJob>());
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
        enum class SearcherState : std::uint8_t { Stopped, StoppingSearch, StoppingBackprop, Searching };

        std::unique_ptr<RootNode> _root;
        SearcherState _state;
        std::mutex _state_mutex;
        std::unique_ptr<std::thread> _searching_thread;
        std::unique_ptr<std::thread> _backpropagating_thread;
        std::unique_ptr<std::thread> _time_managing_thread;
        std::condition_variable _time_managing_variable;
        std::condition_variable _backprop_variable;
        std::ostream *_output;
        std::unique_ptr<MPSCQueue<BackpropJob>> _backprop_queue;
        int _w_time;
        int _b_time;
        int _moves_to_next_time_control;
        int _w_inc;
        int _b_inc;

        void _search() {
            while (_state == SearcherState::Searching) {
                std::unique_ptr<std::vector<ExpandedNode*>> lineage = std::unique_ptr<std::vector<ExpandedNode*>>(new std::vector<ExpandedNode*>());
                ExpandedNode* node = _root.get();
                lineage->push_back(node);
                Board temp_board = Board(_root->_board);
                bool keep_going = true;
                while(keep_going) {
                    std::unique_ptr<Node>* best_child_owner = nullptr;
                    Node* best_child = nullptr;
                    float best_score = -100.0; //max negative float
                    std::unique_ptr<Node>* previous_owner = &(node->_child);
                    for (Node* child : node->children()) {
                        float score = child->score(node->_visits);
                        if (score > best_score) {
                            best_score = score;
                            best_child = child;
                            best_child_owner = previous_owner;
                        }
                        previous_owner = &(child->_sibling);
                    }
                    if (best_child == nullptr) {
                        float result = 0;
                        if (temp_board.is_white_turn()) {
                            result = (node->_score < 0) - (node->_score > 0);
                        } else {
                            result = (node->_score > 0) - (node->_score < 0);
                        }
                        _backprop_queue->enqueue(std::unique_ptr<BackpropJob>(new BackpropJob(result, std::move(lineage), temp_board.is_white_turn())));
                        _backprop_variable.notify_one();
                        keep_going = false;
                    } else if (best_child->is_leaf()) {
                        temp_board.apply_ply(best_child->_ply);
                        float result = ((LeafNode*) best_child)->convert_to_expanded_node(temp_board, best_child_owner);
                        _backprop_queue->enqueue(std::unique_ptr<BackpropJob>(new BackpropJob(result, std::move(lineage), temp_board.is_white_turn())));
                        _backprop_variable.notify_one();
                        keep_going = false;
                    } else {
                        temp_board.apply_ply(best_child->_ply);
                        node = (ExpandedNode*) best_child;
                        lineage->push_back(node);
                    }
                }
            }
        }

        void _manage_time() {
            std::mutex time_managing_mutex;
            std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(time_managing_mutex);
            while (_state == SearcherState::Searching) {
                _time_managing_variable.wait_for(lock, std::chrono::milliseconds(1000));
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
                _state = SearcherState::StoppingBackprop;
                _backprop_variable.notify_one();
                 if (_backpropagating_thread != nullptr && _backpropagating_thread->joinable()) {
                   _backpropagating_thread->join();
                }
                if (!_backprop_queue->empty()) { throw std::runtime_error("Backprop queue is not empty"); }
                _state = SearcherState::Stopped;
                _output_bestmove();
            }
            _state_mutex.unlock();
        }

        void _output_bestmove() {
            std::ostream& output = *_output;
            Ply best_ply = find_best_ply();
            output << "bestmove " << best_ply.to_string() << std::endl;
        }

        void _backpropagate() {
            std::mutex backprop_mutex;
            std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(backprop_mutex);
            while (_state == SearcherState::Searching || _state == SearcherState::StoppingSearch || !_backprop_queue->empty()) {
                _backprop_variable.wait_for(lock, std::chrono::milliseconds(50));  //timeout here in case we miss the closing backprop_variable.notify() in _stop_searching
                while (!_backprop_queue->empty()) {
                    std::unique_ptr<BackpropJob> job = std::move(_backprop_queue->dequeue());
                    job->run();
                }
            }
        }
};
