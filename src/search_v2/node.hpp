#pragma once

#include <math.h>

#include "edge.hpp"
#include "search_constants.hpp"

class Node {

    public:
        int _visits;
        float _score;
        int _num_children;
        std::unique_ptr<Edge[]> _children;

        Node();

        Node(float score, std::unique_ptr<Edge[]> children, int num_children);

        float search_score(int parent_visits, float prior);

        int visits();

        ~Node() { _children.reset(); };

};