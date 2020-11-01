#include "../../chess/ply.cpp"

#include <gtest/gtest.h>

TEST(SquareTest, from_square) {
    Square from = Square(4, 0);
    Square to = Square(4,1);
    Ply p = Ply(from, to);
    ASSERT_EQ(from.to_string(), p.from_square().to_string());
}

TEST(SquareTest, to_square) {
    Square from = Square(4, 0);
    Square to = Square(4,1);
    Ply p = Ply(from, to);
    ASSERT_EQ(to.to_string(), p.to_square().to_string());
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}