#pragma once

#include "leaf_node.hpp"
#include "../evaluation/evaluator.cpp"

class Expander {
    public:
            static void evaluate_and_expand(Board b, ExpandedNode *node, std::vector<ExpandedNode*> lineage) {
                // TODO: replace this placeholder implementation with something real
                Policy policy = Evaluator::evaluate(b);

                std::unique_ptr<Node> previous_leaf = nullptr;
                for (Action a : policy.actions()) {
                    std::unique_ptr<Node> new_node = std::unique_ptr<Node>(new LeafNode(std::move(previous_leaf), a.ply(), a.probability()));
                    previous_leaf = std::move(new_node);
                }
                node->_child = std::move(previous_leaf);

                lineage.push_back(node);
                backpropagate(policy.value(), lineage, b.is_white_turn());
            }

            static void backpropagate(float score, std::vector<ExpandedNode*> lineage, const bool is_white_turn) {
                bool white_to_play = is_white_turn;
                for (std::vector<ExpandedNode*>::reverse_iterator node = lineage.rbegin(); node != lineage.rend(); ++node ) {
                   if (white_to_play) { // potential improvement: skip this part for draws
                        (*node)->_score -= score;
                    } else {
                        (*node)->_score += score;
                    }
                    (*node)->_visits += 1;
                    white_to_play = !white_to_play;
                }
            }
};