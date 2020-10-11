#include "../../chess/square.cpp"

#include <gtest/gtest.h>

TEST(SquareTest, get_row) {
    Square s = Square(4, 0);
    ASSERT_EQ(4, s.get_row());

    s = Square(2, 0);
    ASSERT_EQ(2, s.get_row());
}

TEST(SquareTest, get_col) {
    Square s = Square(0, 4);
    ASSERT_EQ(4, s.get_col());

    s = Square(0, 2);
    ASSERT_EQ(2, s.get_col());
}

TEST(SquareTest, constructor_string) {
    Square s = Square("d7");
    ASSERT_EQ(6, s.get_row());
    ASSERT_EQ(3, s.get_col());

    s = Square("g1");
    ASSERT_EQ(0, s.get_row());
    ASSERT_EQ(6, s.get_col());
}

TEST(SquareTest, to_string) {
    Square s = Square(0, 0);
    ASSERT_EQ("a1", s.to_string());

    s = Square(2, 4);
    ASSERT_EQ("e3", s.to_string());
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}