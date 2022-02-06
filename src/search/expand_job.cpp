#include "backprop_job.cpp"
#include "leaf_node.cpp"

#include "../utility/mpsc_queue.cpp"

#include <set>

class ExpandJob {

    public:
        ExpandJob(bool is_white_turn, std::unique_ptr<Policy> p, LeafNode *l, ExpandedNode* parent, std::unique_ptr<std::vector<ExpandedNode*>> lineage) {
            _is_white_turn = is_white_turn;
            _policy = std::move(p);
            _leaf = l;
            _parent = parent;
            _lineage = std::move(lineage);
        }

        void run(std::set<LeafNode*> *active_nodes, MPSCQueue<BackpropJob>* backprop_queue) {
            float value = Expander::expand(_is_white_turn, _policy.get(), _leaf, _parent);
            active_nodes->erase(_leaf);
            backprop_queue->enqueue(std::unique_ptr<BackpropJob>(new BackpropJob(value, std::move(_lineage), _is_white_turn)));
        }

    private:
        bool _is_white_turn;
        std::unique_ptr<Policy> _policy;
        LeafNode* _leaf;
        ExpandedNode* _parent;
        std::unique_ptr<std::vector<ExpandedNode*>> _lineage;
};