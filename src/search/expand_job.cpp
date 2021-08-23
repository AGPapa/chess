#include "backprop_job.cpp"
#include "mpsc_queue.cpp"
#include "leaf_node.cpp"

#include <set>

class ExpandJob {

    public:
        ExpandJob(Board b, std::unique_ptr<Policy> p, LeafNode *l, ExpandedNode* parent, std::unique_ptr<std::vector<ExpandedNode*>> lineage) {
            _b = b;
            _policy = std::move(p);
            _leaf = l;
            _parent = parent;
            _lineage = std::move(lineage);
        }

        void run(std::set<LeafNode*> *active_nodes, MPSCQueue<BackpropJob>* backprop_queue, std::condition_variable* backprop_variable) {
            float value = Expander::expand(_b, _policy.get(), _leaf, _parent);
            active_nodes->erase(_leaf);
            backprop_queue->enqueue(std::unique_ptr<BackpropJob>(new BackpropJob(value, std::move(_lineage), _b.is_white_turn())));
            backprop_variable->notify_one();
        }

    private:
        Board _b;
        std::unique_ptr<Policy> _policy;
        LeafNode* _leaf;
        ExpandedNode* _parent;
        std::unique_ptr<std::vector<ExpandedNode*>> _lineage;
};