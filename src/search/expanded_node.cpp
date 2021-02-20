#pragma once

#include "node.cpp"
#include "node_iterator.cpp"

class ExpandedNode : public Node {

    public:
        std::unique_ptr<Node> _child;
        int _visits;
        float _score;

        ExpandedNode() : Node() { _visits = 0; _score = 0; _child = nullptr; };

        ExpandedNode(std::unique_ptr<Node> sibling, std::unique_ptr<Node> child, Ply ply, float score, float probability) : Node(std::move(sibling), ply, probability) {
            _child = std::move(child);
            _visits = 0;
            _score = score;
        };

        NodeIterator children() {
            Node* first_child = _child.get();
            return NodeIterator(&first_child);
        }

        float score(int parent_visits) { return _score/_visits + sqrt(parent_visits)/_visits; }

        int visits() { return _visits; };

};