#include "../../evaluation/transformation_layer.cpp"

#include <gtest/gtest.h>

// TODO: test correct mirroring
TEST(TransformationLayerTest, propagate) {
    std::int8_t weights[TransformationLayer::INPUT_DIMENSION * TransformationLayer::SQUARES * 2] = {0};
    weights[64*1+1] = 1;
    weights[64*6+57] = 10;
    std::int8_t biases[2] = { 1, 2 };
    TransformationLayer layer = TransformationLayer(2, weights, biases, false);
    TransformationLayer flipped_layer = TransformationLayer(2, weights, biases, true);

    std::int8_t output[layer.output_dimension()];
    layer.propagate(Board("k7/8/8/8/8/8/8/KN6 w - - 0 100"), output);
    ASSERT_EQ(2, output[0]);
    ASSERT_EQ(2, output[1]);

    flipped_layer.propagate(Board("k7/8/8/8/8/8/8/KN6 w - - 0 100"), output);
    ASSERT_EQ(11, output[0]);
    ASSERT_EQ(2, output[1]);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}