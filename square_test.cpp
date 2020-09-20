#include "square.cpp"

#include <gtest/gtest.h>

TEST(SquareTest, row) {
    Square s = Square(4, 0);
    ASSERT_EQ(4, s.get_row());

    s = Square(2, 0);
    ASSERT_EQ(2, s.get_row());
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}