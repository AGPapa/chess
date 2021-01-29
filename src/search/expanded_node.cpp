#pragma once

#include "node.cpp"
#include "node_iterator.cpp"
#include "../chess/board.cpp"

class ExpandedNode : public Node {

    public:
        std::unique_ptr<Node> _child;
        int _visits;
        float _score;

        ExpandedNode() : Node() { _visits = 1; _score = 0; };

        ExpandedNode(Node *parent, std::unique_ptr<Node> sibling, std::unique_ptr<Node> child, Ply ply, float score, float probability) : Node(parent, std::move(sibling), ply, probability) {
            _child = std::move(child);
            _visits = 1;
            _score = score;
        };

        NodeIterator children() {
            Node* first_child = _child.get();
            return NodeIterator(&first_child);
        }
};