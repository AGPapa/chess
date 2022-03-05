#pragma once

#include "edge.hpp"
#include "search_constants.hpp"

Edge::Edge() {
    _prior = 0;
    _node = nullptr;
};

Edge::Edge(const Ply ply, float prior) {
    _ply = Ply(ply);
    _prior = prior;
    _node = nullptr;
};

bool Edge::is_leaf() { return _node == nullptr; };

float Edge::search_score(int parent_visits) { 
    if (_node == nullptr) {
        return EXPLORATION_FACTOR * _prior * sqrt(parent_visits);
    } else {
        return _node->search_score(parent_visits, _prior);
    }
};

float Edge::visits() { 
    if (_node == nullptr) {
        return 0;
    } else {
        return _node->_visits;
    }
};