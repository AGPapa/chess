#include "expander.cpp"
#include "expanded_node.cpp"
#include "../chess/board.cpp"

class RootNode : public ExpandedNode {

    public:
        Board _board;

        RootNode() : ExpandedNode() {};

        RootNode(const Board board) : ExpandedNode() {
            _board = Board(board);
            Expander::evaluate_and_expand(_board, this);
        };

        RootNode(const Board board, ExpandedNode *node_to_convert) : ExpandedNode(nullptr, std::move(node_to_convert->_child), Ply(), node_to_convert->_score, node_to_convert->_prior) {
            _board = Board(board);
            _board.apply_ply(node_to_convert->_ply);
            _visits = node_to_convert->_visits;
        };

        ~RootNode() { _sibling.reset(); _child.reset(); };

};