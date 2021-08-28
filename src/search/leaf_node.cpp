#pragma once

#include <math.h>

#include "expanded_node.cpp"
#include "../chess/board.cpp"

class LeafNode : public Node {

    public:
        LeafNode() : Node() {};

        LeafNode(std::unique_ptr<Node> sibling, Ply ply, float prior) : Node(std::move(sibling), ply, prior) {};

        bool is_leaf() { return true; };

        float score(int parent_visits) { return EXPLORATION_FACTOR * _prior * sqrt(parent_visits); }

        ~LeafNode() { _sibling.reset(); };
};
