#pragma once

#include "edge.hpp"
#include "search_constants.hpp"

Edge::Edge() {
    _ply = Ply();
    _prior = 0;
};

Edge::Edge(const Ply ply, float prior) {
    _ply = Ply(ply);
    _prior = prior;
};

bool Edge::is_leaf() { return !_node; };

float Edge::search_score(const float sqrt_parent_visits) {
    if (_node) {
        return _node->search_score(sqrt_parent_visits, _prior);
    } else {
        return EXPLORATION_FACTOR * _prior * sqrt_parent_visits;
    }
};

float Edge::visits() { 
    if (_node) {
        return _node->_visits;
    } else {
        return 0;
    }
};