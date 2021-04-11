#include "../../chess/board.cpp"

#include <gtest/gtest.h>

TEST(BoardSignatureTest, is_repetition) {
    Board a = Board("7k/6p1/8/8/8/8/8/7K b - - 0 100");
    Board b = Board("7k/6p1/8/8/8/8/8/7K b - - 17 62");
    ASSERT_EQ(true, a.signature().is_repetition(b.signature()));

    a = Board("7k/6p1/8/8/8/8/8/7K b - - 0 100");
    b = Board("7k/6p1/8/8/8/8/8/6K1 b - - 17 62");
    ASSERT_EQ(false, a.signature().is_repetition(b.signature()));

    a = Board("K7/8/8/8/8/8/8/R3K2R w K - 0 100");
    b = Board("K7/8/8/8/8/8/8/R3K2R w KQ - 0 100");
    ASSERT_EQ(false, a.signature().is_repetition(b.signature()));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}