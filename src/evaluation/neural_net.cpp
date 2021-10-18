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
            for (int i = 0; i < 32*1858; i++) {
                _output_layer_weights[i] = rand() % 256 - 128;
            }
            for (int i = 0; i < 1858; i++) {
                _output_layer_biases[i] = rand() % 64 - 32;
            }
            for (int i = 0; i < 64; i++) {
                std::unique_ptr<TransformationLayer<256>::Weights> weight_ptr(new TransformationLayer<256>::Weights());
                for (int j = 0; j < TransformationLayer<256>::INPUT_DIMENSION * 256; j++) {
                    weight_ptr->weights[j] = rand() % 256 - 128;
                }
                for (int j = 0; j < 256; j++) {
                    weight_ptr->biases[j] = rand() % 64 - 32;
                }
                _friendly_t_layer_weights[i] = std::move(weight_ptr);
            }
            for (int i = 0; i < 64; i++) {
                std::unique_ptr<TransformationLayer<256>::Weights> weight_ptr(new TransformationLayer<256>::Weights());
                for (int j = 0; j < TransformationLayer<256>::INPUT_DIMENSION * 256; j++) {
                    weight_ptr->weights[j] = rand() % 256 - 128;
                }
                for (int j = 0; j < 256; j++) {
                    weight_ptr->biases[j] = rand() % 64 - 32;
                }
                _enemy_t_layer_weights[i] = std::move(weight_ptr);
            }
        };

        std::unique_ptr<Policy> evaluate (const Board b, std::vector<Ply> ply_list) {
            TransformationLayer<256> friendly_t_layer(&_friendly_t_layer_weights, false); // dense 15
            TransformationLayer<256> enemy_t_layer(&_enemy_t_layer_weights, true); // dense 15
            ConcatenationLayer<std::int16_t> concat_layer = ConcatenationLayer<std::int16_t>(&friendly_t_layer, &enemy_t_layer);
            ActivationLayer activation_layer_1 = ActivationLayer(&concat_layer);
            WeightsLayer dense_layer_2 = WeightsLayer(&activation_layer_1, 0, _full_layer_1_weights, _full_layer_1_biases); // dense 16
            ActivationLayer activation_layer_2 = ActivationLayer(&dense_layer_2);
            WeightsLayer dense_layer_3 = WeightsLayer(&activation_layer_2, 0, _full_layer_2_weights, _full_layer_2_biases); // dense 17
            ActivationLayer activation_layer_3 = ActivationLayer(&dense_layer_3);
            OutputLayer output_layer = OutputLayer(&activation_layer_3, _output_layer_weights, _output_layer_biases); // combined head

            std::int16_t output[1858] = { 0 };
            if (b.is_white_turn()) {
                output_layer.propagate(b, ply_list, output);
            } else {
                std::vector<Ply> mirror_ply_list;
                mirror_ply_list.reserve(ply_list.size());
                for (Ply p : ply_list) {
                    mirror_ply_list.push_back(p.mirror());
                }
                output_layer.propagate(b.mirror(), mirror_ply_list, output);
            }

           std::unique_ptr<Policy> pol = std::unique_ptr<Policy>(new Policy(output[0] / 32767.0));

           if (b.is_white_turn()) {
                for (Ply p : ply_list) {
                    pol->add_action(p, output[policy_map.at(p)] / 32767.0);
                }
           } else {
                for (Ply p : ply_list) {
                    pol->add_action(p, output[policy_map.at(p.mirror())] / 32767.0);
                }
           }
           return pol;
        }

    private:
        std::int8_t _full_layer_1_weights[512*32];
        std::int8_t _full_layer_1_biases[32];
        std::int8_t _full_layer_2_weights[32*32];
        std::int8_t _full_layer_2_biases[32];
        std::int8_t _output_layer_weights[32*1858];
        std::int8_t _output_layer_biases[1858];

        std::map<int, std::unique_ptr<TransformationLayer<256>::Weights>> _friendly_t_layer_weights;
        std::map<int, std::unique_ptr<TransformationLayer<256>::Weights>> _enemy_t_layer_weights;


        static std::unique_ptr<TransformationLayer<256>::Weights> _load_weights (int king) {
            std::unique_ptr<TransformationLayer<256>::Weights> weight_ptr(new TransformationLayer<256>::Weights());

            srand(king);

            for (int i = 0; i < TransformationLayer<256>::INPUT_DIMENSION * 256; i++) {
                weight_ptr->weights[i] = rand() % 256 - 128;
            }
            for (int i = 0; i < 256; i++) { //only 16 instead of 256
                weight_ptr->biases[i] = rand() % 64 - 32;
            }
            return std::move(weight_ptr);
        }


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
};