#include "bitboard.cpp"

#include <gtest/gtest.h>

TEST(BitboardTest, set_bit) { 
    Bitboard b;
    ASSERT_EQ(0, b.get_bit(0,0));
    b.set_bit(0,0);
    ASSERT_EQ(1, b.get_bit(0,0));

    
    ASSERT_EQ(0, b.get_bit(6,4));
    b.set_bit(6,4);
    ASSERT_EQ(1, b.get_bit(6,4));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}