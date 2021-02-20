#include "expanded_node.cpp"
#include "../chess/board.cpp"

class LeafNode : public Node {

    public:
        LeafNode() : Node() {};

        LeafNode(Node* parent, std::unique_ptr<Node> sibling, Ply ply, float probability) : Node(parent, std::move(sibling), ply, probability) {};

        bool is_leaf() { return true; };

        void convert_to_expanded_node(Board b, std::unique_ptr<Node> *owner) {
            std::unique_ptr<Node> new_node = std::unique_ptr<Node>(new ExpandedNode(_parent, std::move(_sibling), nullptr, _ply, 0.0, _probability));
            evaluate_and_expand(b, (ExpandedNode*) new_node.get());
            (*owner) = std::move(new_node);
        }

        float score() { return _probability + sqrt(_parent->visits()); }

        static void evaluate_and_expand(Board b, ExpandedNode *node) {
            // TODO: replace this placeholder implementation with something real
            float score = 0.01;

            std::unique_ptr<Node> previous_leaf = nullptr;
            for (Ply p : b.generate_potential_plies()) {
                std::unique_ptr<Node> new_node = std::unique_ptr<Node>(new LeafNode(node, std::move(previous_leaf), p, 0.1));
                previous_leaf = std::move(new_node);
            }
            node->_child = std::move(previous_leaf);

            backpropagate(score, node, b.is_white_turn());
        }

        static void backpropagate(float score, ExpandedNode *node, bool white_to_play) {
            while (node != nullptr) {
                if (white_to_play) { // potential improvement: skip this part for draws
                    node->_score += score;
                } else {
                    node->_score -= score;
                }
                node->_visits += 1;
                node = (ExpandedNode*) node->_parent;
                white_to_play = !white_to_play;
            }
        }
};