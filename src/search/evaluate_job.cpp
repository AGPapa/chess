#include "expand_job.cpp"
#include "../evaluation/evaluator.cpp"

class EvaluateJob {

    public:
        EvaluateJob() {};

        EvaluateJob(const Board curr_b, const Board prev_b, const Ply p, Edge *leaf, std::shared_ptr<std::vector<Node*>> lineage) {
            _curr_board = curr_b;
            _prev_board = prev_b;
            _ply = p;
            _leaf = leaf;
            _lineage = std::move(lineage);
        }

        void run(MPSCQueue<ExpandJob>* expand_queue) {
           expand_queue->enqueue(ExpandJob(_prev_board.is_white_turn(), std::move(Evaluator::evaluate(_curr_board, _prev_board, _ply)), _leaf, _lineage));
        }


    private:
        Board _curr_board;
        Board _prev_board;
        Ply _ply;
        Edge* _leaf;
        std::shared_ptr<std::vector<Node*>> _lineage;
};