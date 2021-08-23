#pragma once

#include "leaf_node.cpp"
#include "../evaluation/evaluator.cpp"

class Expander {
    public:
        static float evaluate_and_expand(Board b, ExpandedNode *node) {
            return _expand(b, Evaluator::evaluate(b).get(), node);
        }

        static float expand(Board b, Policy* policy, LeafNode *leaf, ExpandedNode *parent) {
            std::unique_ptr<Node> new_node = std::unique_ptr<Node>(new ExpandedNode(std::move(leaf->_sibling), nullptr, leaf->_ply, 0.0, leaf->_prior));
            float value = Expander::_expand(b, policy, (ExpandedNode*) new_node.get());
            if (parent->_child.get() == leaf) {
                parent->_child = std::move(new_node);
            } else {
                Node* previous_node = nullptr;
                for (Node* child : parent->children()) {
                    if (child == leaf) {
                        previous_node->_sibling = std::move(new_node);
                        break;
                    }
                    previous_node = child;
                }
            }
            return value;
        }

    private:
        static float _expand(Board b, Policy* policy, ExpandedNode *node) {
            std::unique_ptr<Node> previous_leaf = nullptr;
            for (Action a : policy->actions()) {
                std::unique_ptr<Node> new_node = std::unique_ptr<Node>(new LeafNode(std::move(previous_leaf), a.ply(), a.probability()));
                previous_leaf = std::move(new_node);
            }
            node->_child = std::move(previous_leaf);

            node->_visits += 1;
            if (b.is_white_turn()) {
                node->_score -= policy->value();
            } else {
                node->_score += policy->value();
            }
            return policy->value();
        }
};
