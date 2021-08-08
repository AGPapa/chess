#include "../../evaluation/transformation_layer.cpp"

#include <gtest/gtest.h>

/*
  weights are saved in different order than in other layers
    input 0 - [output 0, output 1, output 2]
    input 1 - [output 0, output 1, output 2]
*/
std::unique_ptr<TransformationLayer<9>::Weights> load_weights (int king) {
  std::unique_ptr<TransformationLayer<9>::Weights> weight_ptr(new TransformationLayer<9>::Weights());
  for (int i = 0; i < TransformationLayer<9>::INPUT_DIMENSION * 9; i++) {
    weight_ptr->weights[i] = 0;
  }
  // sets biases
  weight_ptr->biases[0] = 1;
  weight_ptr->biases[1] = 2;
  // sets weights
  if (king == 2) {
    weight_ptr->weights[(64*1+1)*9] = 1;    //b1 friendly knight
    weight_ptr->weights[(64*6+57)*9] = 10;  //b8 enemy knight
    weight_ptr->weights[(64*10+58)*9] = 20; //c8 enemy king
  }
  return std::move(weight_ptr);
}

TEST(TransformationLayerTest, propagate) {
    Cache<int, TransformationLayer<9>::Weights> cache = Cache<int, TransformationLayer<9>::Weights>(5);

    TransformationLayer<9> layer = TransformationLayer<9>(&cache, load_weights, false);
    TransformationLayer<9> flipped_layer = TransformationLayer<9>(&cache, load_weights, true);

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