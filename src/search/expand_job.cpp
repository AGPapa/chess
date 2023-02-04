#include "backprop_job.cpp"
#include "node.cpp"
#include "expander.cpp"

#include "../evaluation/policy.cpp"
#include "../utility/mpsc_queue.cpp"
#include "../utility/bank.cpp"

#include <unordered_set>

class ExpandJob {

    public:
        ExpandJob() {}

        ExpandJob(bool is_white_turn, std::shared_ptr<Policy> p, Edge *l, int lineage_bank_index) {
            _is_white_turn = is_white_turn;
            _policy = p;
            _leaf = l;
            _lineage_bank_index = lineage_bank_index;
        }

        void run(std::unordered_set<Edge*> *active_nodes, Bank<std::vector<Node*>>* lineage_bank) {
            float value = Expander::expand(_is_white_turn, _policy.get(), _leaf);
            active_nodes->erase(_leaf);
            Expander::backprop(_is_white_turn, lineage_bank->access(_lineage_bank_index), value);
            lineage_bank->access(_lineage_bank_index)->clear();
            lineage_bank->release(_lineage_bank_index);
        }

    private:
        bool _is_white_turn;
        std::shared_ptr<Policy> _policy;
        Edge* _leaf;
        int _lineage_bank_index;
};