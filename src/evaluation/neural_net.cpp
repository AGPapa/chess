#include <stdlib.h>
#include <fstream>

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

            load_transform_weights(&_friendly_t_layer_weights, (int) sizeof(_friendly_t_layer_weights.at(0)->weights)/sizeof(_friendly_t_layer_weights.at(0)->weights[0]), "filename_quant_dense.bin");
            load_transform_weights(&_enemy_t_layer_weights, (int) sizeof(_enemy_t_layer_weights.at(0)->weights)/sizeof(_enemy_t_layer_weights.at(0)->weights[0]), "filename_quant_dense_1.bin");
            load_weights(_full_layer_1_weights, sizeof(_full_layer_1_weights)/sizeof(_full_layer_1_weights[0]), "filename_quant_dense_2.bin");
            load_weights(_full_layer_2_weights, sizeof(_full_layer_2_weights)/sizeof(_full_layer_2_weights[0]), "filename_quant_dense_3.bin");
            load_weights(_output_layer_weights, sizeof(_output_layer_weights)/sizeof(_output_layer_weights[0]), "filename_quant_dense_4.bin");
            for (int i = 0; i < 128; i++) {
                _full_layer_1_biases[i] = 0;
            }
            for (int i = 0; i < 512; i++) {
                _full_layer_2_biases[i] = 0;
            }
            for (int i = 0; i < 1858; i++) {
                _output_layer_biases[i] = 0;
            }

            _friendly_t_layer = TransformationLayer<256>(&_friendly_t_layer_weights, false); // dense 15
            _enemy_t_layer= TransformationLayer<256>(&_enemy_t_layer_weights, true); // dense 15
            _concat_layer = ConcatenationLayer<std::int16_t>(&_friendly_t_layer, &_enemy_t_layer);
            _activation_layer_1 = ActivationLayer(&_concat_layer);
            _dense_layer_2 = WeightsLayer(&_activation_layer_1, 128, _full_layer_1_weights, _full_layer_1_biases); // dense 16
            _activation_layer_2 = ActivationLayer(&_dense_layer_2);
            _dense_layer_3 = WeightsLayer(&_activation_layer_2, 512, _full_layer_2_weights, _full_layer_2_biases); // dense 17
            _activation_layer_3 = ActivationLayer(&_dense_layer_3);
            _output_layer = OutputLayer(&_activation_layer_3, _output_layer_weights, _output_layer_biases); // combined head
        };

        std::unique_ptr<Policy> evaluate (const Board b, std::vector<Ply> ply_list) {
            std::int16_t output[1858] = { 0 };
            if (b.is_white_turn()) {
                _output_layer.propagate(b, ply_list, output);
            } else {
                std::vector<Ply> mirror_ply_list;
                mirror_ply_list.reserve(ply_list.size());
                for (Ply p : ply_list) {
                    mirror_ply_list.push_back(p.mirror());
                }
                _output_layer.propagate(b.mirror(), mirror_ply_list, output);
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
        std::int8_t _full_layer_1_weights[512*128];
        std::int8_t _full_layer_1_biases[128];
        std::int8_t _full_layer_2_weights[128*512];
        std::int8_t _full_layer_2_biases[512];
        std::int8_t _output_layer_weights[512*1858];
        std::int8_t _output_layer_biases[1858];

        std::map<int, std::unique_ptr<TransformationLayer<256>::Weights>> _friendly_t_layer_weights;
        std::map<int, std::unique_ptr<TransformationLayer<256>::Weights>> _enemy_t_layer_weights;

        TransformationLayer<256> _friendly_t_layer;
        TransformationLayer<256> _enemy_t_layer;
        ConcatenationLayer<std::int16_t> _concat_layer;
        ActivationLayer _activation_layer_1;
        WeightsLayer _dense_layer_2;
        ActivationLayer _activation_layer_2;
        WeightsLayer _dense_layer_3;
        ActivationLayer _activation_layer_3;
        OutputLayer _output_layer;

        void load_weights(std::int8_t weights[], int size, std::string filename) {
            std::ifstream bin_file("../src/evaluation/weights/" + filename, std::ios::binary);
            if (bin_file.good()) {
                bin_file.read((char *) weights, size);
                bin_file.close();
            } else {
                throw std::runtime_error("Failed to open weights file " + filename);
            }
        }

        void load_transform_weights(std::map<int, std::unique_ptr<TransformationLayer<256>::Weights>> *weights, int size, std::string filename) {
            std::ifstream bin_file("../src/evaluation/weights/" + filename, std::ios::binary);
            if (bin_file.good()) {
                for (int i = 0; i < 64; i++) {
                    std::unique_ptr<TransformationLayer<256>::Weights> weight_ptr(new TransformationLayer<256>::Weights());
                    bin_file.read((char *) weight_ptr->weights, size);
                    for (int j = 0; j < 256; j++) {
                        weight_ptr->biases[j] = 0;
                    }
                    (*weights)[i] = std::move(weight_ptr);
                }
                bin_file.close();
            } else {
                throw std::runtime_error("Failed to open weights file " + filename);
            }
        }

           // transformation layer
           // dense layer ~80k -> 512 ("dense 15") (two ~40K->256)
           // relu
           // dense layer 512 -> 128  ("dense 16")
           // relu
           // dense layer 128->512  ("dense 17")
           // relu
           // dense layer (512->3) ("value_head")
           // policy head (512->1858) ("policy_head")

           // policy head (512->1861) ("combined_head"?)
};