#include <cstdint>

#include "layer.cpp"

class InitialLayer : public Layer {

    public:

        InitialLayer(int output_dimension, std::int16_t* output) {
            _output_dimension = output_dimension;
            _output = output;
        }

        //TODO: make sure this doesn't leak memory
        const void propagate(std::int16_t* output) {
            for (int i = 0; i < _output_dimension; i++) {
                output[i] = _output[i];
            }
        }

        const int output_dimension() {
            return _output_dimension;
        }

    private:
        int _output_dimension;
        std::int16_t* _output;
};