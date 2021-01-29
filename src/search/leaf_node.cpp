#include "expanded_node.cpp"
#include "../chess/board.cpp"

class LeafNode : public Node {

    public:
        LeafNode() : Node() {};

        LeafNode(Node* parent, std::unique_ptr<Node> sibling, Ply ply, float probability) : Node(parent, std::move(sibling), ply, probability) {};

        bool is_leaf() { return true; };

        void convert_to_expanded_node(Board b, std::unique_ptr<Node> *owner) {
            std::unique_ptr<Node> new_node = std::unique_ptr<Node>(new ExpandedNode(_parent, std::move(_sibling), nullptr, Ply(), 0.0, _probability));
            evaluate_and_expand(b, (ExpandedNode*) new_node.get());
            (*owner) = std::move(new_node);
        }

        static void evaluate_and_expand(Board b, ExpandedNode *node) {
            // TODO: replace this placeholder implementation with something real

            std::unique_ptr<Node> previous_leaf;
            for (Ply p : b.generate_potential_plies()) {
                std::unique_ptr<Node> new_node = std::unique_ptr<Node>(new LeafNode(node, std::move(previous_leaf), p, rand()));
                previous_leaf = std::move(new_node);
            }
            node->_child = std::move(previous_leaf);
            //TODO: backwards pass
        }
};