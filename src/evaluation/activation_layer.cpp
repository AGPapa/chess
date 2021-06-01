#include <cstdint>
#include <algorithm>
#include <iostream>

#include "layer.cpp"

#if defined(USE_NEON)
#include <arm_neon.h>
#endif

class ActivationLayer : public Layer {

    public:

        ActivationLayer(Layer* previous_layer) {
            _previous_layer = previous_layer;
        }

        const void propagate(const Board b, std::int16_t* output) {
            int input_dimension = _previous_layer->output_dimension();

            std::int16_t input[input_dimension];
            _previous_layer->propagate(b, input);

            int i = 0;

            #if defined(USE_NEON)
            const short transfer_size = 4;
	        short segments = input_dimension / transfer_size;
            i = segments * transfer_size;
            const int16x4_t Zero = {0};
            for(short i = 0; i < segments; i++) {
    	        short offset = i * transfer_size;
       	        int16x4_t input_vector = vld1_s16(input + offset);      // Load vector elements to registers
                int16x4_t output_vector = vmax_s16(input_vector, Zero); // Max vector elements with zero
                vst1_s16(output, output_vector);                        // Store vector elements in memory
            }
            #endif

            for (; i < input_dimension; i++) {
                  output[i] = (std::int16_t) std::max(0, std::min(32767, (int) input[i])); // TODO: investigate why stockfish shifts the bits
            }
        }

        const int output_dimension() {
            return _previous_layer->output_dimension();
        }

    private:
        Layer* _previous_layer;
};