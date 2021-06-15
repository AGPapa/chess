#include "../../evaluation/neural_net.cpp"

#include <gtest/gtest.h>

TEST(NeuralNetTest, evaluate) {
    NeuralNet net = NeuralNet();
    net.evaluate(Board::default_board(), Board::default_board().generate_potential_plies());
    ASSERT_EQ(5, 5);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}