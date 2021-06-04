#include "../../evaluation/weights_layer.cpp"
#include "../../evaluation/mock_layer.cpp"

#include <gtest/gtest.h>

#if defined(USE_NEON)
#include <arm_neon.h>
#endif

TEST(WeightsLayerTest, propagate) {
    std::int16_t input[5] = { -1, 0, 1, 2, 3 };
    MockLayer init = MockLayer(5, input);


    std::int16_t weights[10] = { 0, 0, 0, 2, 0, 0, 0, 0, 3, 1 };
    std::int16_t biases[2] = { 1, 2 };

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