#include "backprop_job.cpp"
#include "mpsc_queue.cpp"
#include "leaf_node.cpp"

class ExpandJob {

    public:
        ExpandJob(Board b, std::unique_ptr<Policy> p, LeafNode *l, std::unique_ptr<Node> *owner, std::unique_ptr<std::vector<ExpandedNode*>> lineage) {
            _b = b;
            _policy = std::move(p);
            _leaf = l;
            _owner = owner;
            _lineage = std::move(lineage);
        }

        void run(MPSCQueue<BackpropJob>* backprop_queue, std::condition_variable* backprop_variable) {
            float value = Expander::expand(_b, _policy.get(), _leaf, _owner);

            backprop_queue->enqueue(std::unique_ptr<BackpropJob>(new BackpropJob(value, std::move(_lineage), _b.is_white_turn())));
            backprop_variable->notify_one();
        }

    private:
        Board _b;
        std::unique_ptr<Policy> _policy;
        LeafNode* _leaf;
        std::unique_ptr<Node> *_owner;
        std::unique_ptr<std::vector<ExpandedNode*>> _lineage;
};