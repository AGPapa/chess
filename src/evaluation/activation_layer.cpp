#include <cstdint>
#include <algorithm>
#include <iostream>

#include "layer.cpp"

#if defined(USE_NEON)
#include <arm_neon.h>
#endif

class ActivationLayer : public Layer<std::int8_t> {

    public:

        ActivationLayer(Layer<std::int16_t>* previous_layer) {
            _previous_layer = previous_layer;
        }

        void propagate(const Board b, std::int8_t* output) const {
            int input_dimension = _previous_layer->output_dimension();

            std::int16_t input[input_dimension];
            _previous_layer->propagate(b, input);

            int i = 0;

            #if defined(USE_NEON)
            const short transfer_size = 8;
	        short segments = input_dimension / transfer_size;
            i = segments * transfer_size;
            const int8x8_t Zero = {0};
            for(short x = 0; x < segments; x++) {
    	        short offset = x * transfer_size;
                int16x8_t input_vector =  vld1q_s16(input + offset);      // Load vector elements to registers
                int8x8_t shifted_vector = vshrn_n_s16(input_vector, 6); // Shifts vector bits right
                int8x8_t output_vector = vmax_s8(shifted_vector, Zero); // Max vector elements with zero
                vst1_s8(output, output_vector);                        // Store vector elements in memory
            }
            #endif

            for (; i < input_dimension; i++) {
                  output[i] = (std::int8_t) std::max(0, std::min(127, input[i] >> 6)); // TODO: investigate why stockfish shifts the bits
            }
        }

        const int output_dimension() const {
            return _previous_layer->output_dimension();
        }

    private:
        Layer<std::int16_t>* _previous_layer;
};