#include "root_node.cpp"
#include "evaluate_job.cpp"

#include "../utility/spmc_queue.cpp"

#include <set>

class SearchJob {

    public:
        SearchJob(RootNode *root) {
            _root = root;
        }

        void run(std::set<Edge*> *active_nodes, SPMCQueue<EvaluateJob>* evaluate_queue) {
            std::unique_ptr<std::vector<Node*>> lineage = std::unique_ptr<std::vector<Node*>>(new std::vector<Node*>());
            Node* node = _root;
            lineage->reserve(50);
            lineage->push_back(node);
            Board temp_board = Board(_root->_board);
            while(true) {
                std::unique_ptr<Node>* best_child_owner = nullptr;
                Edge* best_child = nullptr;
                float best_score = -100.0; //max negative float
                float sqrt_vists = sqrt(node->_visits);
                for (int i = 0; i < node->_num_children; i++) {
                    float score = node->_children[i].search_score(sqrt_vists);
                    if (score > best_score) {
                        best_score = score;
                        best_child = &node->_children[i];
                    }
                }
                if (best_child == nullptr) {
                    float result = 0;
                    if (temp_board.is_white_turn()) {
                        result = (node->_score < 0) - (node->_score > 0);
                    } else {
                        result = (node->_score > 0) - (node->_score < 0);
                    }
                    _virtual_loss(lineage.get());
                    Expander::backprop(temp_board.is_white_turn(), lineage.get(), result);
                    return;
                } else if (best_child->is_leaf()) {
                    if (active_nodes->count(best_child) == 0) { //only evaluate if we're not currently evaluating
                        active_nodes->insert(best_child);
                        _virtual_loss(lineage.get());
                        evaluate_queue->enqueue(EvaluateJob(temp_board, best_child->_ply, best_child, std::move(lineage)));
                    }

                    return;
                } else {
                    temp_board.apply_ply(best_child->_ply);
                    node = best_child->_node.get();
                    lineage->push_back(node);
                }
            }
        }

    private:
        RootNode* _root;

        // aka "virtual loss"
        void _virtual_loss(std::vector<Node*> *lineage) {
            for (Node* n : *lineage) {
                n->_visits += 1;
                n->_score -= 1;
            }
        }
};
