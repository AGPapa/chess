#include "root_node.cpp"
#include "evaluate_job.cpp"

#include <set>

class SearchJob {

    public:
        SearchJob(RootNode *root) {
            _root = root;
        }

        void run(std::set<LeafNode*> *active_nodes, MPSCQueue<BackpropJob>* backprop_queue, std::condition_variable* backprop_variable) {
            std::unique_ptr<std::vector<ExpandedNode*>> lineage = std::unique_ptr<std::vector<ExpandedNode*>>(new std::vector<ExpandedNode*>());
            ExpandedNode* node = _root;
            lineage->push_back(node);
            lineage->reserve(50);
            Board temp_board = Board(_root->_board);
            while(true) {
                std::unique_ptr<Node>* best_child_owner = nullptr;
                Node* best_child = nullptr;
                float best_score = -100.0; //max negative float
                std::unique_ptr<Node>* previous_owner = &(node->_child);
                for (Node* child : node->children()) {
                    float score = child->score(node->_visits);
                    if (score > best_score) {
                        best_score = score;
                        best_child = child;
                        best_child_owner = previous_owner;
                    }
                    previous_owner = &(child->_sibling);
                }
                if (best_child == nullptr) {
                    float result = 0;
                    if (temp_board.is_white_turn()) {
                        result = (node->_score < 0) - (node->_score > 0);
                    } else {
                        result = (node->_score > 0) - (node->_score < 0);
                    }
                    lineage->shrink_to_fit();
                    backprop_queue->enqueue(std::unique_ptr<BackpropJob>(new BackpropJob(result, std::move(lineage), temp_board.is_white_turn())));
                    backprop_variable->notify_one();
                    return;
                } else if (best_child->is_leaf()) {
                    temp_board.apply_ply(best_child->_ply);
                    lineage->shrink_to_fit();
                    LeafNode* best_leaf = (LeafNode *) best_child;
                    if (active_nodes->count(best_leaf) == 0) { //only evaluate if we're not currently evaluating
                        active_nodes->insert(best_leaf);
                        EvaluateJob(temp_board, best_leaf, best_child_owner, std::move(lineage)).run(active_nodes, backprop_queue, backprop_variable);
                    }
                    return;
                } else {
                    temp_board.apply_ply(best_child->_ply);
                    node = (ExpandedNode*) best_child;
                    lineage->push_back(node);
                }
            }
        }

    private:
        RootNode* _root;
};



                