#pragma once

#include "leaf_node.hpp"

class Expander {
    public:
            static void evaluate_and_expand(Board b, ExpandedNode *node, std::vector<ExpandedNode*> lineage) {
                // TODO: replace this placeholder implementation with something real
                float score = 0.01;

                std::unique_ptr<Node> previous_leaf = nullptr;
                for (Ply p : b.generate_potential_plies()) {
                    std::unique_ptr<Node> new_node = std::unique_ptr<Node>(new LeafNode(node, std::move(previous_leaf), p, 0.1));
                    previous_leaf = std::move(new_node);
                }
                node->_child = std::move(previous_leaf);

                lineage.push_back(node);
                backpropagate(score, lineage, b.is_white_turn());
            }

            static void backpropagate(float score, std::vector<ExpandedNode*> lineage, const bool is_white_turn) {
                bool white_to_play = is_white_turn;
                for (std::vector<ExpandedNode*>::reverse_iterator node = lineage.rbegin(); node != lineage.rend(); ++node ) {
                   if (white_to_play) { // potential improvement: skip this part for draws
                        (*node)->_score += score;
                    } else {
                        (*node)->_score -= score;
                    }
                    (*node)->_visits += 1;
                    white_to_play = !white_to_play;
                }
            }
};