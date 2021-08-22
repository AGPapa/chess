#include "expand_job.cpp"

class EvaluateJob {

    public:
        EvaluateJob(const Board b, LeafNode *leaf, std::unique_ptr<Node> *owner, std::unique_ptr<std::vector<ExpandedNode*>> lineage) {
            _board = b;
            _leaf = leaf;
            _owner = owner;
            _lineage = std::move(lineage);
        }

        void run(std::set<LeafNode*> *active_nodes, MPSCQueue<ExpandJob>* expand_queue) {
           expand_queue->enqueue(std::unique_ptr<ExpandJob>(new ExpandJob(_board, std::move(Evaluator::evaluate(_board)), _leaf, _owner, std::move(_lineage))));
        }


    private:
        Board _board;
        LeafNode* _leaf;
        std::unique_ptr<Node> *_owner;
        std::unique_ptr<std::vector<ExpandedNode*>> _lineage;
};