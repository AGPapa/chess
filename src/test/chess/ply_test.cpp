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

TEST(SquareTest, promotion) {
    Ply p = Ply("e2e4");
    ASSERT_EQ(Ply::MiscInfo::NoInfo, p.promotion());

    p = Ply("e7e8q");
    ASSERT_EQ(Ply::MiscInfo::QueenPromote, p.promotion());

    p = Ply("e7e8n");
    ASSERT_EQ(Ply::MiscInfo::KnightPromote, p.promotion());

    p = Ply("e7e8b");
    ASSERT_EQ(Ply::MiscInfo::BishopPromote, p.promotion());

    p = Ply("e7e8r");
    ASSERT_EQ(Ply::MiscInfo::RookPromote, p.promotion());

    Square from = Square(6, 0);
    Square to = Square(7,0);

    p = Ply(from, to, Ply::MiscInfo::QueenPromote);
    ASSERT_EQ(Ply::MiscInfo::QueenPromote, p.promotion());

    p = Ply(from, to, Ply::MiscInfo::KnightPromote);
    ASSERT_EQ(Ply::MiscInfo::KnightPromote, p.promotion());

    p = Ply(from, to, Ply::MiscInfo::BishopPromote);
    ASSERT_EQ(Ply::MiscInfo::BishopPromote, p.promotion());

    p = Ply(from, to, Ply::MiscInfo::RookPromote);
    ASSERT_EQ(Ply::MiscInfo::RookPromote, p.promotion());
}

TEST(PlyTest, operator_equals) {
    Ply a = Ply("e2e4");
    Ply b = Ply(Square("e2"), Square("e4"));
    ASSERT_EQ(true, a == b);

    Ply c = Ply("e2e5");
    ASSERT_EQ(false, a == c);

    Ply d = Ply("e7e8q");
    Ply e = Ply(Square("e7"), Square("e8"), Ply::MiscInfo::QueenPromote);
    ASSERT_EQ(true, d == e);

    Ply f = Ply(Square("e7"), Square("e8"), Ply::MiscInfo::KnightPromote);
    ASSERT_EQ(false, d == f);
}

TEST(PlyTest, operator_not_equals) {
    Ply a = Ply("e2e4");
    Ply b = Ply(Square("e2"), Square("e4"));
    ASSERT_EQ(false, a != b);

    Ply c = Ply("e2e5");
    ASSERT_EQ(true, a != c);

    Ply d = Ply("e7e8q");
    Ply e = Ply(Square("e7"), Square("e8"), Ply::MiscInfo::QueenPromote);
    ASSERT_EQ(false, d != e);

    Ply f = Ply(Square("e7"), Square("e8"), Ply::MiscInfo::KnightPromote);
    ASSERT_EQ(true, d != f);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}