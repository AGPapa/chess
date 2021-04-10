#include "../../chess/castling.cpp"

#include <gtest/gtest.h>

TEST(SquareTest, get_white_kingside) {
    Castling c = Castling(true, true, true, true);
    ASSERT_EQ(true, c.get_white_kingside());

    c = Castling(true, false, false, false);
    ASSERT_EQ(true, c.get_white_kingside());

    c = Castling(false, true, true, true);
    ASSERT_EQ(false, c.get_white_kingside());
}

TEST(SquareTest, get_white_queenside) {
    Castling c = Castling(true, true, true, true);
    ASSERT_EQ(true, c.get_white_queenside());

    c = Castling(false, true, false, false);
    ASSERT_EQ(true, c.get_white_queenside());

    c = Castling(true, false, true, true);
    ASSERT_EQ(false, c.get_white_queenside());
}

TEST(SquareTest, get_black_kingside) {
    Castling c = Castling(true, true, true, true);
    ASSERT_EQ(true, c.get_black_kingside());

    c = Castling(false, false, true, false);
    ASSERT_EQ(true, c.get_black_kingside());

    c = Castling(true, true, false, true);
    ASSERT_EQ(false, c.get_black_kingside());
}

TEST(SquareTest, get_black_queenside) {
    Castling c = Castling(true, true, true, true);
    ASSERT_EQ(true, c.get_black_queenside());

    c = Castling(false, false, false, true);
    ASSERT_EQ(true, c.get_black_queenside());

    c = Castling(true, true, true, false);
    ASSERT_EQ(false, c.get_black_queenside());
}

TEST(SquareTest, no_castle_legal) {
    Castling c = Castling(false, false, false, false);
    ASSERT_EQ(true, c.no_castle_legal());

    c = Castling(true, false, false, false);
    ASSERT_EQ(false, c.no_castle_legal());

    c = Castling(false, true, false, false);
    ASSERT_EQ(false, c.no_castle_legal());

    c = Castling(false, false, true, false);
    ASSERT_EQ(false, c.no_castle_legal());

    c = Castling(false, false, false, true);
    ASSERT_EQ(false, c.no_castle_legal());

    c = Castling(true, true, true, true);
    ASSERT_EQ(false, c.no_castle_legal());
}

TEST(SquareTest, unset_white_kingside) {
    Castling c = Castling(true, true, true, true);
    ASSERT_EQ(true, c.get_white_kingside());

    c.unset_white_kingside();
    ASSERT_EQ(false, c.get_white_kingside());
    ASSERT_EQ(true, c.get_white_queenside());
    ASSERT_EQ(true, c.get_black_kingside());
    ASSERT_EQ(true, c.get_black_queenside());

     c = Castling(true, false, false, false);
    ASSERT_EQ(true, c.get_white_kingside());

    c.unset_white_kingside();
    ASSERT_EQ(false, c.get_white_kingside());
    ASSERT_EQ(false, c.get_white_queenside());
    ASSERT_EQ(false, c.get_black_kingside());
    ASSERT_EQ(false, c.get_black_queenside());
}

TEST(SquareTest, unset_white_queenside) {
    Castling c = Castling(true, true, true, true);
    ASSERT_EQ(true, c.get_white_queenside());

    c.unset_white_queenside();
    ASSERT_EQ(true, c.get_white_kingside());
    ASSERT_EQ(false, c.get_white_queenside());
    ASSERT_EQ(true, c.get_black_kingside());
    ASSERT_EQ(true, c.get_black_queenside());

     c = Castling(false, true, false, false);
    ASSERT_EQ(true, c.get_white_queenside());

    c.unset_white_queenside();
    ASSERT_EQ(false, c.get_white_kingside());
    ASSERT_EQ(false, c.get_white_queenside());
    ASSERT_EQ(false, c.get_black_kingside());
    ASSERT_EQ(false, c.get_black_queenside());
}

TEST(SquareTest, unset_black_kingside) {
    Castling c = Castling(true, true, true, true);
    ASSERT_EQ(true, c.get_black_kingside());

    c.unset_black_kingside();
    ASSERT_EQ(true, c.get_white_kingside());
    ASSERT_EQ(true, c.get_white_queenside());
    ASSERT_EQ(false, c.get_black_kingside());
    ASSERT_EQ(true, c.get_black_queenside());

     c = Castling(false, false, true, false);
    ASSERT_EQ(true, c.get_black_kingside());

    c.unset_black_kingside();
    ASSERT_EQ(false, c.get_white_kingside());
    ASSERT_EQ(false, c.get_white_queenside());
    ASSERT_EQ(false, c.get_black_kingside());
    ASSERT_EQ(false, c.get_black_queenside());
}

TEST(SquareTest, unset_black_queenside) {
    Castling c = Castling(true, true, true, true);
    ASSERT_EQ(true, c.get_black_queenside());

    c.unset_black_queenside();
    ASSERT_EQ(true, c.get_white_kingside());
    ASSERT_EQ(true, c.get_white_queenside());
    ASSERT_EQ(true, c.get_black_kingside());
    ASSERT_EQ(false, c.get_black_queenside());

     c = Castling(false, false, false, true);
    ASSERT_EQ(true, c.get_black_queenside());

    c.unset_black_queenside();
    ASSERT_EQ(false, c.get_white_kingside());
    ASSERT_EQ(false, c.get_white_queenside());
    ASSERT_EQ(false, c.get_black_kingside());
    ASSERT_EQ(false, c.get_black_queenside());
}

TEST(SquareTest, unset_white) {
    Castling c = Castling(true, true, true, true);
    ASSERT_EQ(true, c.get_white_kingside());
    ASSERT_EQ(true, c.get_white_queenside());

    c.unset_white();
    ASSERT_EQ(false, c.get_white_kingside());
    ASSERT_EQ(false, c.get_white_queenside());
    ASSERT_EQ(true, c.get_black_kingside());
    ASSERT_EQ(true, c.get_black_queenside());

     c = Castling(true, true, false, false);
    ASSERT_EQ(true, c.get_white_kingside());
    ASSERT_EQ(true, c.get_white_queenside());

    c.unset_white();
    ASSERT_EQ(false, c.get_white_kingside());
    ASSERT_EQ(false, c.get_white_queenside());
    ASSERT_EQ(false, c.get_black_kingside());
    ASSERT_EQ(false, c.get_black_queenside());
}

TEST(SquareTest, unset_black) {
    Castling c = Castling(true, true, true, true);
    ASSERT_EQ(true, c.get_black_kingside());
    ASSERT_EQ(true, c.get_black_queenside());

    c.unset_black();
    ASSERT_EQ(true, c.get_white_kingside());
    ASSERT_EQ(true, c.get_white_queenside());
    ASSERT_EQ(false, c.get_black_kingside());
    ASSERT_EQ(false, c.get_black_queenside());

     c = Castling(false, false, true, true);
    ASSERT_EQ(true, c.get_black_kingside());
    ASSERT_EQ(true, c.get_black_queenside());

    c.unset_black();
    ASSERT_EQ(false, c.get_white_kingside());
    ASSERT_EQ(false, c.get_white_queenside());
    ASSERT_EQ(false, c.get_black_kingside());
    ASSERT_EQ(false, c.get_black_queenside());
}

TEST(CastlingTest, operator_equals) {
    Castling a = Castling(true, false, false, false);
    Castling b = Castling(true, true, false, false);
    ASSERT_EQ(false, a == b);

    b.unset_white_queenside();
    ASSERT_EQ(true, a == b);
}

TEST(CastlingTest, operator_not_equals) {
    Castling a = Castling(true, false, false, false);
    Castling b = Castling(true, true, false, false);
    ASSERT_EQ(true, a != b);

    b.unset_white_queenside();
    ASSERT_EQ(false, a != b);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}