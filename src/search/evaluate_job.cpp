#include "expand_job.cpp"

class EvaluateJob {

    public:
        EvaluateJob(const Board b, LeafNode *leaf, ExpandedNode* parent, std::unique_ptr<std::vector<ExpandedNode*>> lineage) {
            _board = b;
            _leaf = leaf;
            _parent = parent;
            _lineage = std::move(lineage);
        }

        void run(MPSCQueue<ExpandJob>* expand_queue) {
           expand_queue->enqueue(std::unique_ptr<ExpandJob>(new ExpandJob(_board.is_white_turn(), std::move(Evaluator::evaluate(_board)), _leaf, _parent, std::move(_lineage))));
        }


    private:
        Board _board;
        LeafNode* _leaf;
        ExpandedNode* _parent;
        std::unique_ptr<std::vector<ExpandedNode*>> _lineage;
};