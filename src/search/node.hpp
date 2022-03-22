#pragma once

#include <math.h>

#include "edge.hpp"
#include "search_constants.hpp"

class Node {

    public:
        int _visits;
        float _score;
        short _num_children;
        std::unique_ptr<Edge[]> _children;

        Node();

        Node(float score, std::unique_ptr<Edge[]> children, short num_children);

        float search_score(const float sqrt_parent_visits, const float prior);

        int visits();

        ~Node() { _children.reset(); };

};