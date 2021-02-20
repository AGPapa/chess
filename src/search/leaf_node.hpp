#pragma once

#include "expanded_node.cpp"
#include "../chess/board.cpp"

class LeafNode : public Node {

    public:
        LeafNode() : Node() {};

        LeafNode(std::unique_ptr<Node> sibling, Ply ply, float probability) : Node(std::move(sibling), ply, probability) {};

        bool is_leaf() { return true; };

        void convert_to_expanded_node(Board b, std::unique_ptr<Node> *owner, std::vector<ExpandedNode*> lineage);

        float score(int parent_visits) { return _probability + sqrt(parent_visits); }

};