#include "../../evaluation/transformation_layer.cpp"

#include <gtest/gtest.h>

TEST(TransformationLayerTest, propagate) {
    std::int8_t weights[TransformationLayer::INPUT_DIMENSION * TransformationLayer::SQUARES * 2] = {0};
    weights[TransformationLayer::INPUT_DIMENSION * 2 + 64*1+1] = 1; //our knight
    weights[TransformationLayer::INPUT_DIMENSION * 2 + 64*6+57] = 10; // enemy knight
    weights[TransformationLayer::INPUT_DIMENSION * 2 + 64*10+58] = 20; // enemy king
    std::int8_t biases[2] = { 1, 2 };
    TransformationLayer layer = TransformationLayer(2, weights, biases, false);
    TransformationLayer flipped_layer = TransformationLayer(2, weights, biases, true);

    std::int16_t output[layer.output_dimension()];
    layer.propagate(Board("2k5/8/8/8/8/8/8/1NK5 w - - 0 100"), output);
    ASSERT_EQ(22, output[0]);
    ASSERT_EQ(2, output[1]);

    flipped_layer.propagate(Board("2k5/8/8/8/8/8/8/1NK5 w - - 0 100"), output);
    ASSERT_EQ(31, output[0]);
    ASSERT_EQ(2, output[1]);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}