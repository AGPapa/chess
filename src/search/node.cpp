#pragma once

#include "../chess/board.cpp"

class Node : public std::enable_shared_from_this<Node>{

    public:
        std::unique_ptr<Node> _sibling;
        Node* _parent;
        Ply _ply;
        float _probability;

        Node() {};

        Node(Node* parent, std::unique_ptr<Node> sibling, const Ply ply, float probability) {
            _parent = parent;
            _sibling = std::move(sibling);
            _ply = Ply(ply);
            _probability = probability;
        };

        virtual bool is_leaf() { return false; };
};