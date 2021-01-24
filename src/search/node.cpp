#pragma once

#include "../chess/board.cpp"

class Node : public std::enable_shared_from_this<Node>{

    public:
        std::unique_ptr<Node> _sibling;
        Node* _parent;
        Ply _ply;

        Node() {};

        Node(Node* parent, std::unique_ptr<Node> sibling, const Ply ply) {
            _parent = parent;
            _sibling = std::move(sibling);
            _ply = Ply(ply);
        };

        virtual bool is_leaf() { return false; };
};