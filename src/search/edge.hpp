#pragma once

#include "../chess/board.cpp"

class Node;

class Edge {

    public:
        std::unique_ptr<Node> _node;
        Ply _ply;
        float _prior;

        Edge();

        Edge(const Ply ply, float prior);

        bool is_leaf();

        float search_score(const float sqrt_parent_visits);

        float visits();

        void operator=(const Edge& other) {
            _ply = other._ply;
            _prior = other._prior;
        }

        ~Edge() { _node.reset(); };
};