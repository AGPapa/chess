#include "../../evaluation/transformation_layer.cpp"

#include <gtest/gtest.h>

std::unique_ptr<TransformationLayer<2>::Weights> load_weights (int king) {
  std::unique_ptr<TransformationLayer<2>::Weights> weight_ptr(new TransformationLayer<2>::Weights());
  for (int i = 0; i < TransformationLayer<2>::INPUT_DIMENSION * 2; i++) {
    weight_ptr->weights[i] = 0;
  }
  // sets biases
  weight_ptr->biases[0] = 1;
  weight_ptr->biases[1] = 2;
  // sets weights
  if (king == 2) {
    weight_ptr->weights[64*1+1] = 1;
    weight_ptr->weights[64*6+57] = 10;
    weight_ptr->weights[64*10+58] = 20;
  }
  return std::move(weight_ptr);
}

TEST(TransformationLayerTest, propagate) {
    Cache<int, TransformationLayer<2>::Weights> cache = Cache<int, TransformationLayer<2>::Weights>(5);

    TransformationLayer<2> layer = TransformationLayer<2>(&cache, load_weights, false);
    TransformationLayer<2> flipped_layer = TransformationLayer<2>(&cache, load_weights, true);

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