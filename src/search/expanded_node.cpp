#pragma once

#include <math.h>

#include "node.cpp"
#include "node_iterator.cpp"
#include "search_constants.hpp"

class ExpandedNode : public Node {

    public:
        std::unique_ptr<Node> _child;
        int _visits;
        float _score;

        ExpandedNode() : Node() { _visits = 0; _score = 0; _child = nullptr; };

        ExpandedNode(std::unique_ptr<Node> sibling, std::unique_ptr<Node> child, Ply ply, float score, float prior) : Node(std::move(sibling), ply, prior) {
            _child = std::move(child);
            _visits = 0;
            _score = score;
        };

        NodeIterator children() {
            Node* first_child = _child.get();
            return NodeIterator(&first_child);
        }

        float score(int parent_visits) { return _score/_visits + exploration_factor * _prior * sqrt(parent_visits)/_visits; }

        int visits() { return _visits; };

        virtual ~ExpandedNode() { _sibling.reset(); _child.reset(); };
};