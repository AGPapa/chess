#include "expand_job.cpp"

class EvaluateJob {

    public:
        EvaluateJob(const Board b, const Ply p, LeafNode *leaf, ExpandedNode* parent, std::unique_ptr<std::vector<ExpandedNode*>> lineage) {
            _prev_board = b;
            _ply = p;
            _leaf = leaf;
            _parent = parent;
            _lineage = std::move(lineage);
        }

        void run(MPSCQueue<ExpandJob>* expand_queue) {
           expand_queue->enqueue(std::unique_ptr<ExpandJob>(new ExpandJob(_prev_board.is_white_turn(), std::move(Evaluator::evaluate(_prev_board, _ply)), _leaf, _parent, std::move(_lineage))));
        }


    private:
        Board _prev_board;
        Ply _ply;
        LeafNode* _leaf;
        ExpandedNode* _parent;
        std::unique_ptr<std::vector<ExpandedNode*>> _lineage;
};