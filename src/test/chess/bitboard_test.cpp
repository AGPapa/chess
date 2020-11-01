#include "../../chess/bitboard.cpp"

#include <gtest/gtest.h>

TEST(BitboardTest, set_square_ints) {
    Bitboard b;
    ASSERT_EQ(0, b.get_square(0,0));
    b.set_square(0,0);
    ASSERT_EQ(1, b.get_square(0,0));

    
    ASSERT_EQ(0, b.get_square(6,4));
    b.set_square(6,4);
    ASSERT_EQ(1, b.get_square(6,4));
}

TEST(BitboardTest, set_square_Square) {
    Bitboard b;
    Square s = Square(3,2);
    ASSERT_EQ(0, b.get_square(3,2));
    b.set_square(s);
    ASSERT_EQ(1, b.get_square(3,2));
    ASSERT_EQ(1, b.get_square(s));
}

TEST(BitboardTest, set_square_if_ints) {
    Bitboard b;
    ASSERT_EQ(0, b.get_square(0,0));
    b.set_square_if(0,0, false);
    ASSERT_EQ(0, b.get_square(0,0));
    b.set_square_if(0,0, true);
    ASSERT_EQ(1, b.get_square(0,0));
    b.set_square_if(0,0, false);
    ASSERT_EQ(1, b.get_square(0,0));

    ASSERT_EQ(0, b.get_square(6,4));
    b.set_square_if(6,4, false);
    ASSERT_EQ(0, b.get_square(6,4));
    b.set_square_if(6,4, true);
    ASSERT_EQ(1, b.get_square(6,4));
    b.set_square_if(6,4, false);
    ASSERT_EQ(1, b.get_square(6,4));
}

TEST(BitboardTest, set_square_if_Square) {
    Bitboard b;
    Square s = Square(3,2);
    ASSERT_EQ(0, b.get_square(3,2));
    b.set_square_if(s, false);
    ASSERT_EQ(0, b.get_square(3,2));
    ASSERT_EQ(0, b.get_square(s));

    b.set_square_if(s, true);
    ASSERT_EQ(1, b.get_square(3,2));
    ASSERT_EQ(1, b.get_square(s));

    b.set_square_if(s, false);
    ASSERT_EQ(1, b.get_square(3,2));
    ASSERT_EQ(1, b.get_square(s));
}

TEST(BitboardTest, unset_square_ints) {
    Bitboard b;
    ASSERT_EQ(0, b.get_square(0,0));
    b.set_square(0,0);
    ASSERT_EQ(1, b.get_square(0,0));
    b.unset_square(0,0);
    ASSERT_EQ(0, b.get_square(0,0));

    ASSERT_EQ(0, b.get_square(6,4));
    b.set_square(6,4);
    ASSERT_EQ(1, b.get_square(6,4));
    b.unset_square(6,4);
    ASSERT_EQ(0, b.get_square(6,4));
}

TEST(BitboardTest, unset_square_Square) {
    Bitboard b;
    Square s = Square(3,2);
    ASSERT_EQ(0, b.get_square(3,2));
    b.set_square(s);
    ASSERT_EQ(1, b.get_square(3,2));
    ASSERT_EQ(1, b.get_square(s));
    b.unset_square(s);
    ASSERT_EQ(0, b.get_square(3,2));
    ASSERT_EQ(0, b.get_square(s));
}

TEST(BitboardTest, empty) {
    Bitboard a;
    ASSERT_EQ(true, a.empty());

    a.set_square(6,4);
    ASSERT_EQ(false, a.empty());

    a.unset_square(6,4);
    ASSERT_EQ(true, a.empty());
}

TEST(BitboardTest, squarewise_or) {
    Bitboard a;
    a.set_square(6,4);

    Bitboard b;
    b.set_square(2,3);

    Bitboard c = squarewise_or(a, b);
    ASSERT_EQ(1, c.get_square(6,4));
    ASSERT_EQ(1, c.get_square(2,3));
}

TEST(BitboardTest, squarewise_and) {
    Bitboard a;
    a.set_square(6,4);

    Bitboard b;
    b.set_square(2,3);
    b.set_square(6,4);

    Bitboard c = squarewise_and(a, b);
    ASSERT_EQ(1, c.get_square(6,4));
    ASSERT_EQ(0, c.get_square(2,3));
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}