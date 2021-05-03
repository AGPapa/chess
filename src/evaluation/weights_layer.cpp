#include <cstdint>
#include <iostream>

#include "layer.cpp"

class WeightsLayer : public Layer {

    public:

        WeightsLayer(Layer* previous_layer, int output_dimension, std::int16_t* weights, std::int16_t* biases) {
            _previous_layer = previous_layer;
            _output_dimension = output_dimension;
            _weights = weights;
            _biases = biases;
        }

        const void propagate(const Board b, std::int16_t* output) {
            int input_dimension = _previous_layer->output_dimension();
            std::int16_t input[input_dimension];
            _previous_layer->propagate(b, input);

            for (int i = 0; i < _output_dimension; i++) {
                std::int16_t sum = _biases[i];
                for (int j = 0; j < input_dimension; j++) {
                    sum += _weights[i * input_dimension + j] * input[j];
                }
                output[i] = sum;
            }
        }

        const int output_dimension() {
            return _output_dimension;
        }

    private:
        Layer* _previous_layer;
        int _output_dimension;
        std::int16_t* _weights;
        std::int16_t* _biases;
};