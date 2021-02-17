#include "root_node.cpp"

class Searcher {

    public:

        Searcher() {
            _root = std::unique_ptr<RootNode>(new RootNode(Board::default_board()));
        };

        Ply find_best_ply(int nodes) {
            for (int i = 0; i < nodes; i++) {
                ExpandedNode* node = _root.get();
                Board temp_board = Board(_root->_board);
                bool keep_going = true;
                while(keep_going) {
                    std::unique_ptr<Node>* best_child_owner = nullptr;
                    Node* best_child = nullptr;
                    float best_score = -100.0; //max negative float
                    std::unique_ptr<Node>* previous_owner = &(node->_child);
                    for (Node* child : node->children()) {
                        float score = child->score();
                        if (score > best_score) {
                            best_score = score;
                            best_child = child;
                            best_child_owner = previous_owner;
                        }
                        previous_owner = &(child->_sibling);
                    }
                    if (best_child == nullptr) {
                        //TODO: Handle terminal node
                        keep_going = false;
                    } else if (best_child->is_leaf()) {
                        temp_board.apply_ply(best_child->_ply);
                        ((LeafNode*) best_child)->convert_to_expanded_node(temp_board, best_child_owner);
                        keep_going = false;
                    } else {
                        temp_board.apply_ply(best_child->_ply);
                        node = (ExpandedNode*) best_child;
                    }
                }
            }
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
                        //TODO: handle this case
                        break;
                    } else {
                        Board new_board = _root->_board;
                        new_board.apply_ply(p);
                        _root = std::unique_ptr<RootNode>(new RootNode(new_board, ((ExpandedNode*) child)));
                        break;
                    }
                }
            }
        }
    
    private:
        std::unique_ptr<RootNode> _root;
};