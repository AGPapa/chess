#include "../../chess/board.cpp"

#include <gtest/gtest.h>
#include <algorithm>

TEST(BoardTest, to_fen) {
    Bitboard w_pawns = Bitboard(0x0000000080007F00);
    Bitboard w_rooks = Bitboard(0x0000000000000089);
    Bitboard w_knights = Bitboard(0x0000000000000042);
    Bitboard w_bishops = Bitboard(0x000000000000002C);
    Square w_king = Square(0x04);

    Bitboard b_pawns = Bitboard(0x807F008000000000);
    Bitboard b_rooks = Bitboard(0x8900000000000000);
    Bitboard b_knights = Bitboard(0x4200000000000000);
    Bitboard b_bishops = Bitboard(0x2C00000000000000);
    Square b_king = Square(0x3C);

    Castling c = Castling(true, true, true, true);

    Board b = Board(w_pawns, w_rooks, w_knights, w_bishops, w_king,
                    b_pawns, b_rooks, b_knights, b_bishops, b_king,
                    true, c, 0, 2);

    ASSERT_EQ("rnbqkbnr/ppppppp1/8/7p/7P/8/PPPPPPP1/RNBQKBNR w KQkq h6 0 2", b.to_fen());
}

TEST(BoardTest, default_board) {
    Board b = Board::default_board();
    ASSERT_EQ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", b.to_fen());
}

TEST(BoardTest, from_fen) {
    Board a = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    ASSERT_EQ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", a.to_fen());

    Board b = Board("rnbqkbnr/ppppppp1/8/7p/7P/8/PPPPPPP1/RNBQKBNR w KQkq h6 0 2");
    ASSERT_EQ("rnbqkbnr/ppppppp1/8/7p/7P/8/PPPPPPP1/RNBQKBNR w KQkq h6 0 2", b.to_fen());

    Board c = Board("4k2r/6r1/8/8/8/8/3R4/R3K3 w Qk - 10 100");
    ASSERT_EQ("4k2r/6r1/8/8/8/8/3R4/R3K3 w Qk - 10 100", c.to_fen());
}

TEST(BoardTest, white_pieces) {
    Board a = Board::default_board();
    ASSERT_EQ(Bitboard(0x000000000000FFFF), a.white_pieces());

    a.apply_ply(Ply("a2a3"));
    ASSERT_EQ(Bitboard(0x000000000001FEFF), a.white_pieces());

    Board b = Board("k7/8/8/8/7P/8/8/K7 w - h3 0 100");
    ASSERT_EQ(Bitboard(0x0000000080000001), b.white_pieces());

    Board c = Board("k7/8/8/7p/8/8/8/7K w - h6 0 100");
    ASSERT_EQ(Bitboard(0x0000000000000080), c.white_pieces());
}

TEST(BoardTest, black_pieces) {
    Board a = Board::default_board();
    ASSERT_EQ(Bitboard(0xFFFF000000000000), a.black_pieces());

    a.apply_ply(Ply("a2a3"));
    ASSERT_EQ(Bitboard(0xFFFF000000000000), a.black_pieces());

    Board b = Board("k7/8/8/8/7P/8/8/K7 w - h3 0 100");
    ASSERT_EQ(Bitboard(0x0100000000000000), b.black_pieces());

    Board c = Board("k7/8/8/7p/8/8/8/7K w - h6 0 100");
    ASSERT_EQ(Bitboard(0x0100008000000000), c.black_pieces());
}

TEST(BoardTest, all_pieces) {
    Board a = Board::default_board();
    ASSERT_EQ(Bitboard(0xFFFF00000000FFFF), a.all_pieces());

    a.apply_ply(Ply("a2a3"));
    ASSERT_EQ(Bitboard(0xFFFF00000001FEFF), a.all_pieces());

    Board b = Board("k7/8/8/8/7P/8/8/K7 w - h3 0 100");
    ASSERT_EQ(Bitboard(0x0100000080000001), b.all_pieces());

    Board c = Board("k7/8/8/7p/8/8/8/7K w - h6 0 100");
    ASSERT_EQ(Bitboard(0x0100008000000080), c.all_pieces());
}

TEST(BoardTest, apply_ply) {
    Board a = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    a.apply_ply(Ply("e2e4"));
    ASSERT_EQ("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", a.to_fen());
    a.apply_ply(Ply("d7d5"));
    ASSERT_EQ("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2", a.to_fen());
    a.apply_ply(Ply("e4d5"));
    ASSERT_EQ("rnbqkbnr/ppp1pppp/8/3P4/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 2", a.to_fen());
    a.apply_ply(Ply("d8d5"));
    ASSERT_EQ("rnb1kbnr/ppp1pppp/8/3q4/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 3", a.to_fen());
    a.apply_ply(Ply("e1e2"));
    ASSERT_EQ("rnb1kbnr/ppp1pppp/8/3q4/8/8/PPPPKPPP/RNBQ1BNR b kq - 1 3", a.to_fen());
    a.apply_ply(Ply("e8d8"));
    ASSERT_EQ("rnbk1bnr/ppp1pppp/8/3q4/8/8/PPPPKPPP/RNBQ1BNR w - - 2 4", a.to_fen());


    Board b = Board("8/k3PPPP/8/8/8/8/K3pppp/8 w - - 0 100");
    b.apply_ply(Ply("h7h8q"));
    ASSERT_EQ("7Q/k3PPP1/8/8/8/8/K3pppp/8 b - - 0 100", b.to_fen());
    b.apply_ply(Ply("h2h1q"));
    ASSERT_EQ("7Q/k3PPP1/8/8/8/8/K3ppp1/7q w - - 0 101", b.to_fen());
    b.apply_ply(Ply("g7g8n"));
    ASSERT_EQ("6NQ/k3PP2/8/8/8/8/K3ppp1/7q b - - 0 101", b.to_fen());
    b.apply_ply(Ply("g2g1n"));
    ASSERT_EQ("6NQ/k3PP2/8/8/8/8/K3pp2/6nq w - - 0 102", b.to_fen());
    b.apply_ply(Ply("f7f8b"));
    ASSERT_EQ("5BNQ/k3P3/8/8/8/8/K3pp2/6nq b - - 0 102", b.to_fen());
    b.apply_ply(Ply("f2f1b"));
    ASSERT_EQ("5BNQ/k3P3/8/8/8/8/K3p3/5bnq w - - 0 103", b.to_fen());
    b.apply_ply(Ply("e7e8r"));
    ASSERT_EQ("4RBNQ/k7/8/8/8/8/K3p3/5bnq b - - 0 103", b.to_fen());
    b.apply_ply(Ply("e2e1r"));
    ASSERT_EQ("4RBNQ/k7/8/8/8/8/K7/4rbnq w - - 0 104", b.to_fen());

    Board c = Board("rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    c.apply_ply(Ply("e1g1"));
    ASSERT_EQ("rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4", c.to_fen());
    c.apply_ply(Ply("e8g8"));
    ASSERT_EQ("rnbq1rk1/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 w - - 6 5", c.to_fen());

    Board d = Board("r3kbnr/pppqpppp/2n5/3p1b2/3P1B2/2N5/PPPQPPPP/R3KBNR w KQkq - 6 5");
    d.apply_ply(Ply("e1c1"));
    ASSERT_EQ("r3kbnr/pppqpppp/2n5/3p1b2/3P1B2/2N5/PPPQPPPP/2KR1BNR b kq - 7 5", d.to_fen());
    d.apply_ply(Ply("e8c8"));
    ASSERT_EQ("2kr1bnr/pppqpppp/2n5/3p1b2/3P1B2/2N5/PPPQPPPP/2KR1BNR w - - 8 6", d.to_fen());

    Board e = Board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 100");
    e.apply_ply(Ply("a1a2"));
    ASSERT_EQ("r3k2r/8/8/8/8/8/R7/4K2R b Kkq - 1 100", e.to_fen());
    e.apply_ply(Ply("a8a7"));
    ASSERT_EQ("4k2r/r7/8/8/8/8/R7/4K2R w Kk - 2 101", e.to_fen());
    e.apply_ply(Ply("h1h2"));
    ASSERT_EQ("4k2r/r7/8/8/8/8/R6R/4K3 b k - 3 101", e.to_fen());
    e.apply_ply(Ply("h8h7"));
    ASSERT_EQ("4k3/r6r/8/8/8/8/R6R/4K3 w - - 4 102", e.to_fen());

    Board f = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    f.apply_ply(Ply("a2a4"));
    ASSERT_EQ("rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1", f.to_fen());
    f.apply_ply(Ply("h7h5"));
    ASSERT_EQ("rnbqkbnr/ppppppp1/8/7p/P7/8/1PPPPPPP/RNBQKBNR w KQkq h6 0 2", f.to_fen());
    f.apply_ply(Ply("a4a5"));
    ASSERT_EQ("rnbqkbnr/ppppppp1/8/P6p/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 2", f.to_fen());
    f.apply_ply(Ply("b7b5"));
    ASSERT_EQ("rnbqkbnr/p1ppppp1/8/Pp5p/8/8/1PPPPPPP/RNBQKBNR w KQkq b6 0 3", f.to_fen());
    f.apply_ply(Ply("a5b6"));
    ASSERT_EQ("rnbqkbnr/p1ppppp1/1P6/7p/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 3", f.to_fen());
    f.apply_ply(Ply("h5h4"));
    ASSERT_EQ("rnbqkbnr/p1ppppp1/1P6/8/7p/8/1PPPPPPP/RNBQKBNR w KQkq - 0 4", f.to_fen());
    f.apply_ply(Ply("g2g4"));
    ASSERT_EQ("rnbqkbnr/p1ppppp1/1P6/8/6Pp/8/1PPPPP1P/RNBQKBNR b KQkq g3 0 4", f.to_fen());
    f.apply_ply(Ply("h4g3"));
    ASSERT_EQ("rnbqkbnr/p1ppppp1/1P6/8/8/6p1/1PPPPP1P/RNBQKBNR w KQkq - 0 5", f.to_fen());
}

TEST(BoardTest, generate_potential_plies) {
    Board a = Board("8/7p/7P/1k6/8/1K6/8/8 w - - 0 100");
    std::vector<Ply> expected = { Ply("b3b4"), Ply("b3c4"), Ply("b3c3"), Ply("b3c2"), Ply("b3b2"), Ply("b3a2"), Ply("b3a3"), Ply("b3a4") };
    std::sort(expected.begin(), expected.end());
    std::vector<Ply> actual = a.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
    a.apply_ply(Ply("b3b2"));
    expected = { Ply("b5b6"), Ply("b5c6"), Ply("b5c5"), Ply("b5c4"), Ply("b5b4"), Ply("b5a4"), Ply("b5a5"), Ply("b5a6") };
    std::sort(expected.begin(), expected.end());
    actual = a.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);

    Board b = Board("7k/7p/8/8/8/8/7P/7K w - - 0 100");
    expected = { Ply("h1g1"), Ply("h1g2"), Ply("h2h3"), Ply("h2h4") };
    std::sort(expected.begin(), expected.end());
    actual = b.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
    b.apply_ply(Ply("h1g1"));
    expected = { Ply("h8g8"), Ply("h8g7"), Ply("h7h6"), Ply("h7h5") };
    std::sort(expected.begin(), expected.end());
    actual = b.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);

    Board c = Board("7k/P6p/8/7P/7p/p1P5/P1Pp3P/7K w - - 0 100");
    expected = { Ply("h1g1"), Ply("h1g2"), Ply("h2h3"), Ply("h5h6"), Ply("c3c4"), Ply("a7a8q"), Ply("a7a8n"), Ply("a7a8r"), Ply("a7a8b") };
    std::sort(expected.begin(), expected.end());
    actual = c.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
    c.apply_ply(Ply("h1g1"));
    expected = { Ply("h8g8"), Ply("h8g7"), Ply("h7h6"), Ply("h4h3"), Ply("d2d1q"), Ply("d2d1n"), Ply("d2d1r"), Ply("d2d1b") };
    std::sort(expected.begin(), expected.end());
    actual = c.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);

    Board d = Board("b6k/PP6/8/8/8/8/8/K7 w - - 0 100");
    expected = { Ply("a1b1"), Ply("a1b2"), Ply("a1a2"), Ply("b7b8q"), Ply("b7b8n"), Ply("b7b8r"), Ply("b7b8b"), Ply("b7a8q"), Ply("b7a8n"), Ply("b7a8r"), Ply("b7a8b") };
    std::sort(expected.begin(), expected.end());
    actual = d.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}