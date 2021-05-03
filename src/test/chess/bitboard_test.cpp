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
    b.unset_square(s);
    ASSERT_EQ(0, b.get_square(3,2));
    ASSERT_EQ(0, b.get_square(s));
}

TEST(BitboardTest, get_square) {
    Bitboard b;
    Square s = Square(2, 3);
    b.set_square(s);
    ASSERT_EQ(true, b.get_square(s));
    ASSERT_EQ(true, b.get_square(2, 3));
    ASSERT_EQ(true, b.get_square(19));
}

TEST(BitboardTest, mirror) {
    Bitboard a;
    a.set_square(0, 0);
    a.set_square(1, 1);
    a.set_square(2, 2);
    a.set_square(3, 3);
    a.set_square(4, 4);
    a.set_square(5, 5);
    a.set_square(6, 6);
    a.set_square(7, 7);
    Bitboard b = a.mirror();

    ASSERT_EQ(false, b.get_square(0,0));
    ASSERT_EQ(false, b.get_square(1,1));
    ASSERT_EQ(false, b.get_square(2,2));
    ASSERT_EQ(false, b.get_square(3,3));
    ASSERT_EQ(false, b.get_square(4,4));
    ASSERT_EQ(false, b.get_square(5,5));
    ASSERT_EQ(false, b.get_square(6,6));
    ASSERT_EQ(false, b.get_square(7,7));

    ASSERT_EQ(true, b.get_square(7,0));
    ASSERT_EQ(true, b.get_square(6,1));
    ASSERT_EQ(true, b.get_square(5,2));
    ASSERT_EQ(true, b.get_square(4,3));
    ASSERT_EQ(true, b.get_square(3,4));
    ASSERT_EQ(true, b.get_square(2,5));
    ASSERT_EQ(true, b.get_square(1,6));
    ASSERT_EQ(true, b.get_square(0,7));
}

TEST(BitboardTest, empty) {
    Bitboard a;
    ASSERT_EQ(true, a.empty());

    a.set_square(6,4);
    ASSERT_EQ(false, a.empty());

    a.unset_square(6,4);
    ASSERT_EQ(true, a.empty());
}

TEST(BitboardTest, count) {
    Bitboard a;
    ASSERT_EQ(0, a.count());

    a.set_square(6, 4);
    ASSERT_EQ(1, a.count());

    a.set_square(7, 7);
    ASSERT_EQ(2, a.count());

    a.unset_square(6, 4);
    ASSERT_EQ(1, a.count());
}

TEST(BitboardTest, squarewise_not) {
    Bitboard a = Bitboard(0x0123456789ABCDEF);

    ASSERT_EQ(Bitboard(0xFEDCBA9876543210), a.squarewise_not());
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

TEST(BitboardTest, operator_equals) {
    Bitboard a = Bitboard(0xFFFF00000000FFFF);
    Bitboard b = Bitboard(0xFFFF00000000FFFF);
    Bitboard c = Bitboard(0xFFFF00000001FEFF);

    ASSERT_EQ(true, a == b);
    ASSERT_EQ(false, a == c);

    a.set_square(2, 0);
    a.unset_square(1, 0);
    ASSERT_EQ(true, a == c);
}

TEST(BitboardTest, operator_not_equals) {
    Bitboard a = Bitboard(0xFFFF00000000FFFF);
    Bitboard b = Bitboard(0xFFFF00000000FFFF);
    Bitboard c = Bitboard(0xFFFF00000001FEFF);

    ASSERT_EQ(false, a != b);
    ASSERT_EQ(true, a != c);

    a.set_square(2, 0);
    a.unset_square(1, 0);
    ASSERT_EQ(false, a != c);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}