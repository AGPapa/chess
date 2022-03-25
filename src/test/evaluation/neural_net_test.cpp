#include "../../evaluation/neural_net.cpp"

#include <gtest/gtest.h>

TEST(NeuralNetTest, evaluate) {
    NeuralNet net = NeuralNet();
    net.evaluate(Board::default_board(), Board::default_board().generate_potential_plies());
    ASSERT_EQ(5, 5);
}

TEST(NeuralNetTest, mirrored) {
    NeuralNet net = NeuralNet();
    Board white = Board("k7/p7/8/8/8/8/P7/K7 w - - 10 100");
    Board black = Board("k7/p7/8/8/8/8/P7/K7 b - - 10 100");
    std::unique_ptr<Policy> p_white = net.evaluate(white, white.generate_potential_plies());
    std::unique_ptr<Policy> p_black = net.evaluate(black, black.generate_potential_plies());
    ASSERT_EQ(p_white->_value, p_black->_value);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}