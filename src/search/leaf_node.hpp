#pragma once

#include "expanded_node.cpp"
#include "../chess/board.cpp"

class LeafNode : public Node {

    public:
        LeafNode() : Node() {};

        LeafNode(Node* parent, std::unique_ptr<Node> sibling, Ply ply, float probability) : Node(parent, std::move(sibling), ply, probability) {};

        bool is_leaf() { return true; };

        void convert_to_expanded_node(Board b, std::unique_ptr<Node> *owner);

        float score() { return _probability + sqrt(_parent->visits()); }

};