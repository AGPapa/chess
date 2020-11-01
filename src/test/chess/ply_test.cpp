#include "../../chess/ply.cpp"

#include <gtest/gtest.h>

TEST(SquareTest, square_constructor) {
    Square from = Square(4, 0);
    Square to = Square(4,1);
    Ply p = Ply(from, to);
    ASSERT_EQ(from.to_string(), p.from_square().to_string());
    ASSERT_EQ(to.to_string(), p.to_square().to_string());
    ASSERT_EQ("a5b5", p.to_string());
}

TEST(SquareTest, string_constructor) {
    Ply p = Ply("e2e4");
    ASSERT_EQ("e2", p.from_square().to_string());
    ASSERT_EQ("e4", p.to_square().to_string());
    ASSERT_EQ("e2e4", p.to_string());
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}