#include <cstdint>
#include <iostream>

#include "layer.cpp"

#if defined(USE_NEON)
#include <arm_neon.h>
#endif

class WeightsLayer : public Layer<std::int16_t>  {

    public:

        WeightsLayer(Layer<std::int8_t>* previous_layer, int output_dimension, std::int8_t* weights, std::int8_t* biases) {
            _previous_layer = previous_layer;
            _output_dimension = output_dimension;
            _weights = weights;
            _biases = biases;
        }

        const void propagate(const Board b, std::int16_t* output) {
            int input_dimension = _previous_layer->output_dimension();
            std::int8_t input[input_dimension];
            _previous_layer->propagate(b, input);

            for (int i = 0; i < _output_dimension; i++) {
                int j = 0;
                std::int16_t sum = _biases[i];
                #if defined(USE_NEON)
                    const short transfer_size = 8;
                    short segments = input_dimension / transfer_size;
                    j = segments * transfer_size;

                    int16x8_t sums_vector = {0};
                    std::int16_t sums[transfer_size];
                    for(short x = 0; x < segments; x++) {
                        short offset = x * transfer_size;
                        int8x8_t input_vector = vld1_s8(input + offset); // load vector elements to registers
                        int8x8_t weights_vector = vld1_s8(_weights + i * input_dimension + offset); // load vector elements to registers
                   
                        sums_vector = vmlal_s8(sums_vector, input_vector, weights_vector); // sums + (input dot weights)
                    }
                    vst1q_s16(sums, sums_vector); // store vector elements in memory
                    sum += sums[0] + sums[1] + sums[2] + sums[3] + sums[4] + sums[5] + sums[6] + sums[7];
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
        Layer<std::int8_t>* _previous_layer;
        int _output_dimension;
        std::int8_t* _weights;
        std::int8_t* _biases;
};