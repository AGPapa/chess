#pragma once

#include "../chess/board.cpp"

class Layer {
    public:
        virtual const void propagate(Board b, std::int16_t* output) { return; };
        virtual const int output_dimension() { return 0; };
};