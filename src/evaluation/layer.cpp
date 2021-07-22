#pragma once

#include "../chess/board.cpp"

template<typename OutputType>
class Layer {
    public:
        virtual void propagate(const Board b, OutputType* output) { return; };
        virtual const int output_dimension() const { return 0; };
};