#include <cstdint>
#include <algorithm>
#include <iostream>

#include "layer.cpp"

class ActivationLayer : public Layer {

    public:

        ActivationLayer(Layer* previous_layer) {
            _previous_layer = previous_layer;
        }

        const void propagate(const Board b, std::int16_t* output) {
            int input_dimension = _previous_layer->output_dimension();

            std::int16_t input[input_dimension];
             _previous_layer->propagate(b, input);

            for (int i = 0; i < input_dimension; i++) {
                  output[i] = (std::int16_t) std::max(0, std::min(32767, (int) input[i])); // TODO: investigate why stockfish shifts the bits
            }
        }

        const int output_dimension() {
            return _previous_layer->output_dimension();
        }

    private:
        Layer* _previous_layer;
};