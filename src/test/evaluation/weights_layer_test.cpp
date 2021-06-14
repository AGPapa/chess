#include "../../evaluation/weights_layer.cpp"
#include "../../evaluation/mock_layer.cpp"

#include <gtest/gtest.h>

#if defined(USE_NEON)
#include <arm_neon.h>
#endif

TEST(WeightsLayerTest, propagate) {
    std::int8_t input[9] = { -1, 0, 1, 2, 3, 0, 0, 0, 0 };
    MockLayer<std::int8_t> init = MockLayer<std::int8_t>(9, input);


    std::int8_t weights[18] = { 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0 };
    std::int8_t biases[2] = { 1, 2 };

    WeightsLayer layer = WeightsLayer(&init, 2, weights, biases);

    std::int16_t output[layer.output_dimension()];
    layer.propagate(Board::default_board(), output);
    ASSERT_EQ(5, output[0]);
    ASSERT_EQ(11, output[1]);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}