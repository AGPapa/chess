#include "../../evaluation/concatenation_layer.cpp"
#include "../../evaluation/mock_layer.cpp"

#include <gtest/gtest.h>

TEST(ConcatenationLayerTest, propagate) {
    std::int16_t first_layer_input[5] = { -64, 0, 64, 128, 192 };
    std::int16_t second_layer_input[4] = { 0, 64, 128, 192 };
    MockLayer<std::int16_t> first_layer = MockLayer<std::int16_t>(5, first_layer_input);
    MockLayer<std::int16_t> second_layer = MockLayer<std::int16_t>(4, second_layer_input);

    ConcatenationLayer<std::int16_t> concat = ConcatenationLayer<std::int16_t>(&first_layer, &second_layer);

    std::int16_t output[concat.output_dimension()];
    concat.propagate(Board::default_board(), output);
    ASSERT_EQ(-64, output[0]);
    ASSERT_EQ(0, output[1]);
    ASSERT_EQ(64, output[2]);
    ASSERT_EQ(128, output[3]);
    ASSERT_EQ(192, output[4]);
    ASSERT_EQ(0, output[5]);
    ASSERT_EQ(64, output[6]);
    ASSERT_EQ(128, output[7]);
    ASSERT_EQ(192, output[8]);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}