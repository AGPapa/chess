#include <stdlib.h>

#include "policy.cpp"
#include "transformation_layer.cpp"
#include "concatenation_layer.cpp"
#include "weights_layer.cpp"
#include "activation_layer.cpp"
#include "output_layer.cpp"
#include "../chess/board.cpp"

class NeuralNet {

    public:
        NeuralNet() {
            srand(2018);

            for (int i = 0; i < TransformationLayer::INPUT_DIMENSION * TransformationLayer::SQUARES * 16; i++) { //only 16 instead of 256
                _friendly_transform_weights[i] = rand() % 256 - 128;
            }
            for (int i = 0; i < 16; i++) { //only 16 instead of 256
                _friendly_transform_biases[i] = rand() % 64 - 32;
            }
            for (int i = 0; i < TransformationLayer::INPUT_DIMENSION * TransformationLayer::SQUARES * 16; i++) { //only 16 instead of 256
                _enemy_transform_weights[i] = rand() % 256 - 128;
            }
            for (int i = 0; i < 16; i++) { //only 16 instead of 256
                _enemy_transform_biases[i] = rand() % 64 - 32;
            }
            for (int i = 0; i < 512*32; i++) {
                _full_layer_1_weights[i] = rand() % 256 - 128;
            }
            for (int i = 0; i < 32; i++) {
                _full_layer_1_biases[i] = rand() % 64 - 32;
            }
            for (int i = 0; i < 32*32; i++) {
                _full_layer_2_weights[i] = rand() % 256 - 128;
            }
            for (int i = 0; i < 32; i++) {
                _full_layer_2_biases[i] = rand() % 64 - 32;
            }
            for (int i = 0; i < 32*1859; i++) {
                _output_layer_weights[i] = rand() % 256 - 128;
            }
            for (int i = 0; i < 1859; i++) {
                _output_layer_biases[i] = rand() % 64 - 32;
            }
        };

        std::unique_ptr<Policy> evaluate (const Board b, std::vector<Ply> ply_list) const {
            TransformationLayer friendly_t_layer(16, _friendly_transform_weights, _friendly_transform_biases, false); // dense 15
            TransformationLayer enemy_t_layer(16, _enemy_transform_weights, _enemy_transform_biases, true); // dense 15
            ConcatenationLayer<std::int16_t> concat_layer = ConcatenationLayer<std::int16_t>(&friendly_t_layer, &enemy_t_layer);
            ActivationLayer activation_layer_1 = ActivationLayer(&concat_layer);
            WeightsLayer dense_layer_2 = WeightsLayer(&activation_layer_1, 0, _full_layer_1_weights, _full_layer_1_biases); // dense 16
            ActivationLayer activation_layer_2 = ActivationLayer(&dense_layer_2);
            WeightsLayer dense_layer_3 = WeightsLayer(&activation_layer_2, 0, _full_layer_2_weights, _full_layer_2_biases); // dense 17
            ActivationLayer activation_layer_3 = ActivationLayer(&dense_layer_3);
            OutputLayer output_layer = OutputLayer(&activation_layer_3, _output_layer_weights, _output_layer_biases); // combined head

           std::int16_t output[1859] = { 0 };
           output_layer.propagate(b, ply_list, output);

           std::unique_ptr<Policy> pol = std::unique_ptr<Policy>(new Policy(output[0] / 32767.0));
           for (Ply p : ply_list) {
               pol->add_action(p, output[policy_map.at(p)] / 32767.0);
           }
           return pol;
        }

    private:
        std::int8_t _friendly_transform_weights[TransformationLayer::INPUT_DIMENSION * TransformationLayer::SQUARES * 16];
        std::int8_t _friendly_transform_biases[16];
        std::int8_t _enemy_transform_weights[TransformationLayer::INPUT_DIMENSION * TransformationLayer::SQUARES * 16];
        std::int8_t _enemy_transform_biases[16];
        std::int8_t _full_layer_1_weights[512*32];
        std::int8_t _full_layer_1_biases[32];
        std::int8_t _full_layer_2_weights[32*32];
        std::int8_t _full_layer_2_biases[32];
        std::int8_t _output_layer_weights[32*1859];
        std::int8_t _output_layer_biases[1859];


          


           // transformation layer
           // dense layer ~80k -> 512 ("dense 15") (two ~40K->256)
           // relu
           // dense layer 512 -> 32  ("dense 16")
           // relu
           // dense layer 32->32  ("dense 17")
           // relu
           // dense layer (32->1) ("value_head")
           // policy head (32->1858) ("policy_head")

           // policy head (32->1859) ("combined_head"?)
        
           /*
            Input: 2 x 40,965
            layer1: 2 x 256
            layer2: 32
            layer3: 32
            */

           // 10,486,784 <-"should be"
           // 5,804,251  <-is
};