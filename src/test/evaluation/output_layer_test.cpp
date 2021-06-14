#include "../../evaluation/output_layer.cpp"
#include "../../evaluation/mock_layer.cpp"

#include <gtest/gtest.h>

#if defined(USE_NEON)
#include <arm_neon.h>
#endif

TEST(OutputLayerTest, propagate) {
    std::int8_t input[9] = { -1, 0, 1, 2, 3, 0, 0, 0, 0 };
    MockLayer<std::int8_t> init = MockLayer<std::int8_t>(9, input);

    std::int8_t weights[9*1859] = {0};
    std::int8_t biases[1859] = {0};

    std::vector<Ply> ply_list = { Ply("a1a2") };

    weights[0] = 1;
    biases[0] = 10;
    weights[9*policy_map.at(ply_list[0])+2] = 15; 

    OutputLayer layer = OutputLayer(&init, weights, biases);
    std::int16_t output[layer.output_dimension()];
    layer.propagate(Board("7k/8/8/8/8/8/1K6/RK6 w - - 0 100"), ply_list, output);
    ASSERT_EQ(9, output[0]);
    ASSERT_EQ(15, output[policy_map.at(ply_list[0])]);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}