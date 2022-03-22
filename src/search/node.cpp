#pragma once

#include <math.h>

#include "node.hpp"
#include "edge.cpp"


Node::Node() { _visits = 0; _score = 0; _children = nullptr; _num_children = 0; };

Node::Node(float score, std::unique_ptr<Edge[]> children, short num_children) {
    _visits = 1;
    _score = score;
    _children = std::move(children);
    _num_children = num_children;
};

float Node::search_score(const float sqrt_parent_visits, const float prior) { return _score/_visits + EXPLORATION_FACTOR * prior * sqrt_parent_visits/_visits; }

int Node::visits() { return _visits; };
