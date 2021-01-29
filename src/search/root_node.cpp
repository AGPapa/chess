#include "leaf_node.cpp"
#include "expanded_node.cpp"
#include "../chess/board.cpp"

class RootNode : public ExpandedNode {

    public:
        RootNode() : ExpandedNode() {};

        RootNode(const Board board) : ExpandedNode() {
            _board = Board(board);
            LeafNode::evaluate_and_expand(_board, this);
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