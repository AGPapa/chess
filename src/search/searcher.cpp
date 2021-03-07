#include <thread>

#include "root_node.cpp"
#include "leaf_node.cpp"

class Searcher {

    public:

        Searcher() {
            _root = std::unique_ptr<RootNode>(new RootNode(Board::default_board()));
            _state = SearcherState::Stopped;
        };

        Searcher(Board starting_board) {
            _root = std::unique_ptr<RootNode>(new RootNode(starting_board));
            _state = SearcherState::Stopped;
        };

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
            if (_state != SearcherState::Stopped) {
                throw std::runtime_error("Starting search while search is still running");
            }
            _state = SearcherState::Searching;
            _searching_thread = std::unique_ptr<std::thread>(new std::thread(&Searcher::_search, this));
        }

        void stop_searching() {
            _state = SearcherState::Stopping;
            _searching_thread->join();
            _state = SearcherState::Stopped;
        }

    private:
        enum class SearcherState : std::uint8_t { Stopped, Stopping, Searching };

        std::unique_ptr<RootNode> _root;
        SearcherState _state;
        std::unique_ptr<std::thread> _searching_thread;
        int _w_time;
        int _b_time;
        int _moves_to_next_time_control;
        int _w_inc;
        int _b_inc;

        void _search() {
            while (_state == SearcherState::Searching) {
                std::vector<ExpandedNode*> lineage = std::vector<ExpandedNode*>();
                ExpandedNode* node = _root.get();
                lineage.push_back(node);
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
                            if (temp_board.is_white_king_in_check()) { result = 1; }
                        } else {
                            if (temp_board.is_black_king_in_check()) { result = -1; }
                        }
                        Expander::backpropagate(result, lineage, temp_board.is_white_turn());
                        keep_going = false;
                    } else if (best_child->is_leaf()) {
                        temp_board.apply_ply(best_child->_ply);
                        ((LeafNode*) best_child)->convert_to_expanded_node(temp_board, best_child_owner, lineage);
                        keep_going = false;
                    } else {
                        temp_board.apply_ply(best_child->_ply);
                        node = (ExpandedNode*) best_child;
                        lineage.push_back(node);
                    }
                }
            }
        }
};