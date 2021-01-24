#include "expanded_node.cpp"
#include "../chess/board.cpp"

class LeafNode : public Node {

    public:
        LeafNode() : Node() {};

        LeafNode(Node* parent, std::unique_ptr<Node> sibling, Ply ply) : Node(parent, std::move(sibling), ply) {};

        bool is_leaf() { return true; };

        void expand(Board b, std::unique_ptr<Node> *owner) {
            ExpandedNode new_node;
            
            Board board = Board(b);
            std::unique_ptr<Node> previous_leaf;
            Ply previous_ply;
            for (Ply p : board.generate_potential_plies()) {
                std::unique_ptr<Node> node = std::unique_ptr<Node>(new LeafNode(this, std::move(previous_leaf), p));
                previous_leaf = std::move(node);
                previous_ply = p;
            }

            (*owner).reset(new ExpandedNode(_parent, std::move(_sibling), std::move(previous_leaf), previous_ply));
        }
};