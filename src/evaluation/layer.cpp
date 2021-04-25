#pragma once

class Layer {
    public:
        virtual const void propagate(std::int16_t* output) { return; };
        virtual const int output_dimension() { return 0; };
};