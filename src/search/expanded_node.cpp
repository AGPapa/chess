#pragma once

#include "node.cpp"
#include "../chess/board.cpp"

class ExpandedNode : public Node {

    public:
        std::unique_ptr<Node> _child;

        ExpandedNode() : Node() {};

        ExpandedNode(Node *parent, std::unique_ptr<Node> sibling, std::unique_ptr<Node> child, Ply ply) : Node(parent, std::move(sibling), ply) {
            _child = std::move(child);
        };
};