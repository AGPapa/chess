#include "backprop_job.cpp"
#include "node.cpp"
#include "expander.cpp"

#include "../evaluation/policy.cpp"
#include "../utility/mpsc_queue.cpp"

#include <set>

class ExpandJob {

    public:
        ExpandJob(bool is_white_turn, std::unique_ptr<Policy> p, Edge *l, std::unique_ptr<std::vector<Node*>> lineage) {
            _is_white_turn = is_white_turn;
            _policy = std::move(p);
            _leaf = l;
            _lineage = std::move(lineage);
        }

        void run(std::set<Edge*> *active_nodes, MPSCQueue<BackpropJob>* backprop_queue) {
            float value = Expander::expand(_is_white_turn, _policy.get(), _leaf);
            active_nodes->erase(_leaf);
            backprop_queue->enqueue(std::unique_ptr<BackpropJob>(new BackpropJob(value, std::move(_lineage), _is_white_turn)));
        }

    private:
        bool _is_white_turn;
        std::unique_ptr<Policy> _policy;
        Edge* _leaf;
        std::unique_ptr<std::vector<Node*>> _lineage;
};