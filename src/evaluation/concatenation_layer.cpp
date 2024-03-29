#include <cstdint>

#include "layer.cpp"

template<typename OutputType>
class ConcatenationLayer : public Layer<OutputType> {

    public:

        ConcatenationLayer() {}

        ConcatenationLayer(Layer<OutputType>* first_layer, Layer<OutputType>* second_layer) {
            _first_layer = first_layer;
            _second_layer = second_layer;
        }

        void propagate(const Board b, const Board prev_board, const Ply p, OutputType* output) {
            _first_layer->propagate(b, prev_board, p, output);
            _second_layer->propagate(b, prev_board, p, output + _first_layer->output_dimension());
        }

        void propagate(const Board b, OutputType* output) {
            _first_layer->propagate(b, output);
            _second_layer->propagate(b, output + _first_layer->output_dimension());
        }

        const int output_dimension() const {
            return _first_layer->output_dimension() + _second_layer->output_dimension();
        }

    private:
        Layer<OutputType>* _first_layer;
        Layer<OutputType>* _second_layer;
};