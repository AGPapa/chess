#include "expand_job.cpp"
#include "../evaluation/evaluator.cpp"

class EvaluateJob {

    public:
        EvaluateJob(const Board b, const Ply p, Edge *leaf, std::unique_ptr<std::vector<Node*>> lineage) {
            _prev_board = b;
            _ply = p;
            _leaf = leaf;
            _lineage = std::move(lineage);
        }

        void run(MPSCQueue<ExpandJob>* expand_queue) {
           expand_queue->enqueue(std::unique_ptr<ExpandJob>(new ExpandJob(_prev_board.is_white_turn(), std::move(Evaluator::evaluate(_prev_board, _ply)), _leaf, std::move(_lineage))));
        }


    private:
        Board _prev_board;
        Ply _ply;
        Edge* _leaf;
        std::unique_ptr<std::vector<Node*>> _lineage;
};