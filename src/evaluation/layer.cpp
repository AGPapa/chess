#pragma once

#include "../chess/board.cpp"

template<typename OutputType>
class Layer {
    public:
        virtual const void propagate(const Board b, OutputType* output) { return; };
        virtual const int output_dimension() { return 0; };
};