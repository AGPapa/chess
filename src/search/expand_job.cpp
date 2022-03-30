#include "backprop_job.cpp"
#include "node.cpp"
#include "expander.cpp"

#include "../evaluation/policy.cpp"
#include "../utility/mpsc_queue.cpp"

#include <set>

class ExpandJob {

    public:
        ExpandJob() {}

        ExpandJob(bool is_white_turn, std::shared_ptr<Policy> p, Edge *l, std::shared_ptr<std::vector<Node*>> lineage) {
            _is_white_turn = is_white_turn;
            _policy = p;
            _leaf = l;
            _lineage = lineage;
        }

        void run(std::set<Edge*> *active_nodes, MPSCQueue<BackpropJob>* backprop_queue) {
            float value = Expander::expand(_is_white_turn, _policy.get(), _leaf);
            active_nodes->erase(_leaf);
            backprop_queue->enqueue(BackpropJob(value, _lineage, _is_white_turn));
        }

    private:
        bool _is_white_turn;
        std::shared_ptr<Policy> _policy;
        Edge* _leaf;
        std::shared_ptr<std::vector<Node*>> _lineage;
};