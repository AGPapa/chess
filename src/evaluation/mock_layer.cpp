#include <cstdint>

#include "layer.cpp"

template<typename OutputType>
class MockLayer : public Layer<OutputType> {

    public:

        MockLayer(int output_dimension, OutputType* output) {
            _output_dimension = output_dimension;
            _output = output;
        }

        void propagate(const Board b, OutputType* output) const {
            for (int i = 0; i < _output_dimension; i++) {
                output[i] = _output[i];
            }
        }

        const int output_dimension() const {
            return _output_dimension;
        }

    private:
        int _output_dimension;
        OutputType* _output;
};