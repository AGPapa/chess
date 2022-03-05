#pragma once

#include "node.cpp"
#include "../evaluation/evaluator.cpp"

class Expander {
    public:
        static float evaluate_and_expand(Board b, Node *node) {
            std::unique_ptr<Policy> policy = Evaluator::evaluate(b);
            int size = policy->actions().size();
            std::unique_ptr<Edge[]> edges = std::unique_ptr<Edge[]>(new Edge[size]);
            int i = 0;
            for (Action a : policy->actions()) {
                edges[i] = Edge(a.ply(), a.probability());
                i++;
            }
            node->_num_children = size;
            
            node->_children = std::move(edges);
            if (b.is_white_turn()) {
                node->_score = policy->value();
            } else {
                node->_score = -policy->value();
            }
            node->_visits = 1;
            return policy->value();
        }

        static float expand(bool is_white_turn, Policy* policy, Edge *edge) {
            int size = policy->actions().size();
            std::unique_ptr<Edge[]> edges = std::unique_ptr<Edge[]>(new Edge[size]);
            int i = 0;
            for (Action a : policy->actions()) {
                edges[i] = Edge(a.ply(), a.probability());
                i++;
            }
            if (is_white_turn) {
                edge->_node = std::unique_ptr<Node>(new Node(policy->value(), std::move(edges), size));
            } else {
                edge->_node = std::unique_ptr<Node>(new Node(-policy->value(), std::move(edges), size));
            }
            return policy->value();
        }
};
