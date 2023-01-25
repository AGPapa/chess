#include "expander.cpp"
#include "node.cpp"
#include "../chess/board.cpp"

class RootNode : public Node {

    public:
        Board _board;
        BoardHistory _history;

        RootNode() : Node() {};

        RootNode(const Board board, const BoardHistory history) : Node() {
            _board = board;
            _history = history;
            Expander::evaluate_and_expand(_board, this);
        };

        RootNode(const Board board, const BoardHistory history, Ply p, Node *node_to_convert) : Node(node_to_convert->_score, std::move(node_to_convert->_children), node_to_convert->_num_children) {
            _board = board;
            _history = history;
            _board.apply_ply(p, &_history);
            _visits = node_to_convert->_visits;
        };

        ~RootNode() { _children.reset(); };

};