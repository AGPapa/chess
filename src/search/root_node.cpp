#include "leaf_node.cpp"
#include "expanded_node.cpp"
#include "../chess/board.cpp"

class RootNode : public ExpandedNode {

    public:
        RootNode() : ExpandedNode() {};

        RootNode(const Board board) : ExpandedNode() {
            _board = Board(board);
            std::unique_ptr<Node> previous_leaf;
            // TODO: evaluate network to get scores and probailities
            for (Ply p : _board.generate_potential_plies()) {
                std::unique_ptr<Node> node = std::unique_ptr<Node>(new LeafNode(this, std::move(previous_leaf), p, 0.0));
                previous_leaf = std::move(node);
            }
            _child = std::move(previous_leaf);
        };

        RootNode(const Board board, ExpandedNode *node_to_convert) : ExpandedNode(nullptr, nullptr, std::move(node_to_convert->_child), Ply(), node_to_convert->_score, node_to_convert->_probability) {
            _board = Board(board);
            _board.apply_ply(node_to_convert->_ply);

            // Reparent Children
            Node* c = _child.get();
            do
            {
                c->_parent = this;
                c = c->_sibling.get();
            } while (c != nullptr);
        };

    private:
        Board _board;
};