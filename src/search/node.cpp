#pragma once

#include "../chess/board.cpp"

class Node : public std::enable_shared_from_this<Node>{

    public:
        std::unique_ptr<Node> _sibling;
        Ply _ply;
        float _probability;

        Node() {
            _sibling = nullptr;
            _probability = 0;
        };

        Node(std::unique_ptr<Node> sibling, const Ply ply, float probability) {
            _sibling = std::move(sibling);
            _ply = Ply(ply);
            _probability = probability;
        };

        virtual bool is_leaf() { return false; };

        virtual float score(int parent_visits) { return -100.0; };

        virtual int visits() { return 0; };
};