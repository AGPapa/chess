#include <cstdint>
#include <iostream>

#include "layer.cpp"

#if defined(USE_NEON)
#include <arm_neon.h>
#endif

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
                int j = 0;
                std::int16_t sum = _biases[i];
                #if defined(USE_NEON)
                    const short transfer_size = 4;
                    short segments = input_dimension / transfer_size;
                    j = segments * transfer_size;

                    int16x4_t sums = {0};
                    for(short x = 0; x < segments; x++) {
                        short offset = x * transfer_size;
                        int16x4_t input_vector = vld1_s16(input + offset);      // Load vector elements to registers
                        int16x4_t weights_vector = vld1_s16(_weights + i * input_dimension + offset);      // Load vector elements to registers   
                   
                        sums = vmla_s16(sums, input_vector, weights_vector); // sums + (input dot weights)
                    }
                    sum += sums[0] + sums[1] + sums[2] + sums[3];
                #endif

                for (; j < input_dimension; j++) {
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