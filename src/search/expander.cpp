#pragma once

#include "node.cpp"
#include "../evaluation/evaluator.cpp"

class Expander {
    public:
        static float evaluate_and_expand(Board b, Node *node) {
            std::unique_ptr<Policy> policy = Evaluator::evaluate(b);
            int size = policy->_num_actions;
            std::unique_ptr<Edge[]> edges = std::unique_ptr<Edge[]>(new Edge[size]);
            for (int i = 0; i < size; i++) {
                edges[i]._ply = policy->_actions[i].ply();
                edges[i]._prior = policy->_actions[i].probability();
            }
            node->_num_children = size;
            
            node->_children = std::move(edges);
            if (b.is_white_turn()) {
                node->_score = policy->_value;
            } else {
                node->_score = -policy->_value;
            }
            node->_visits = 1;
            return policy->_value;
        }

        static float expand(bool is_white_turn, Policy* policy, Edge *edge) {
            int size = policy->_num_actions;
            std::unique_ptr<Edge[]> edges = std::unique_ptr<Edge[]>(new Edge[size]);
            for (int i = 0; i < size; i++) {
                edges[i]._ply = policy->_actions[i].ply();
                edges[i]._prior = policy->_actions[i].probability();
            }
            if (is_white_turn) {
                edge->_node = std::unique_ptr<Node>(new Node(policy->_value, std::move(edges), size));
            } else {
                edge->_node = std::unique_ptr<Node>(new Node(-policy->_value, std::move(edges), size));
            }
            return policy->_value;
        }
};
