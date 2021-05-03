#include "../../evaluation/weights_layer.cpp"
#include "../../evaluation/mock_layer.cpp"

#include <gtest/gtest.h>

TEST(WeightsLayerTest, propagate) {
    std::int16_t input[4] = { -1, 0, 1, 2 };
    MockLayer init = MockLayer(4, input);


    std::int16_t weights[8] = { 0, 0, 0, 2, 0, 0, 0, 3 };
    std::int16_t biases[4] = { 1, 2 };

    WeightsLayer layer = WeightsLayer(&init, 2, weights, biases);

    std::int16_t output[layer.output_dimension()];
    layer.propagate(Board::default_board(), output);
    ASSERT_EQ(5, output[0]);
    ASSERT_EQ(8, output[1]);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}