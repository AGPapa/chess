#include <cstdint>

#include "layer.cpp"

template<typename OutputType>
class MockLayer : public Layer<OutputType> {

    public:

        MockLayer(int output_dimension, std::int16_t* output) {
            _output_dimension = output_dimension;
            _output = output;
        }

        const void propagate(const Board b, std::int16_t* output) {
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