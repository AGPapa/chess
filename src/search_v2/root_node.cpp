#include "expander.cpp"
#include "node.cpp"
#include "../chess/board.cpp"

class RootNode : public Node {

    public:
        Board _board;

        RootNode() : Node() {};

        RootNode(const Board board) : Node() {
            _board = Board(board);
            Expander::evaluate_and_expand(_board, this);
        };

        RootNode(const Board board, Ply p, Node *node_to_convert) : Node(node_to_convert->_score, std::move(node_to_convert->_children), node_to_convert->_num_children) {
            _board = board;
            _board.apply_ply(p);
            _visits = node_to_convert->_visits;
        };

        ~RootNode() { _children.reset(); };

};