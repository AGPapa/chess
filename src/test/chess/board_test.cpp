#include "../../chess/board.cpp"

#include <gtest/gtest.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>

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

TEST(BoardTest, is_white_turn) {
    Board a = Board::default_board();
    ASSERT_EQ(true, a.is_white_turn());

    a.apply_ply(Ply("e2e4"));
    ASSERT_EQ(false, a.is_white_turn());
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

    Board g = Board("rnbqk2r/8/8/8/8/8/1B7/RN1QK2R w KQkq - 0 20");
    g.apply_ply(Ply("b2h8"));
    ASSERT_EQ("rnbqk2B/8/8/8/8/8/8/RN1QK2R b KQq - 0 20", g.to_fen());
    g.apply_ply(Ply("a8a1"));
    ASSERT_EQ("1nbqk2B/8/8/8/8/8/8/rN1QK2R w K - 0 21", g.to_fen());
}

TEST(BoardTest, square_under_attack) {
    Board a = Board("7k/8/8/8/3nN3/8/8/7K w - - 0 100");
    ASSERT_EQ(true, a.square_under_attack(Square("c5")));
    ASSERT_EQ(true, a.square_under_attack(Square("d6")));
    ASSERT_EQ(true, a.square_under_attack(Square("f6")));
    ASSERT_EQ(true, a.square_under_attack(Square("g5")));
    ASSERT_EQ(true, a.square_under_attack(Square("g3")));
    ASSERT_EQ(true, a.square_under_attack(Square("f2")));
    ASSERT_EQ(true, a.square_under_attack(Square("d2")));
    ASSERT_EQ(false, a.square_under_attack(Square("c2")));
    a.apply_ply(Ply("h1h2"));
    ASSERT_EQ(true, a.square_under_attack(Square("b5")));
    ASSERT_EQ(true, a.square_under_attack(Square("c6")));
    ASSERT_EQ(true, a.square_under_attack(Square("e6")));
    ASSERT_EQ(true, a.square_under_attack(Square("f5")));
    ASSERT_EQ(true, a.square_under_attack(Square("f3")));
    ASSERT_EQ(true, a.square_under_attack(Square("e2")));
    ASSERT_EQ(true, a.square_under_attack(Square("c2")));
    ASSERT_EQ(false, a.square_under_attack(Square("c3")));

    Board b = Board("7k/8/8/8/1p1rR1P1/8/8/7K w - - 0 100");
    ASSERT_EQ(true, b.square_under_attack(Square("e3")));
    ASSERT_EQ(true, b.square_under_attack(Square("e2")));
    ASSERT_EQ(true, b.square_under_attack(Square("e1")));
    ASSERT_EQ(true, b.square_under_attack(Square("e5")));
    ASSERT_EQ(true, b.square_under_attack(Square("e6")));
    ASSERT_EQ(true, b.square_under_attack(Square("e7")));
    ASSERT_EQ(true, b.square_under_attack(Square("e8")));
    ASSERT_EQ(true, b.square_under_attack(Square("f4")));
    ASSERT_EQ(true, b.square_under_attack(Square("d4")));
    ASSERT_EQ(true, b.square_under_attack(Square("g4")));
    ASSERT_EQ(false, b.square_under_attack(Square("h4")));
    ASSERT_EQ(false, b.square_under_attack(Square("c4")));
    b.apply_ply(Ply("h1h2"));
    ASSERT_EQ(true, b.square_under_attack(Square("d3")));
    ASSERT_EQ(true, b.square_under_attack(Square("d2")));
    ASSERT_EQ(true, b.square_under_attack(Square("d1")));
    ASSERT_EQ(true, b.square_under_attack(Square("d5")));
    ASSERT_EQ(true, b.square_under_attack(Square("d6")));
    ASSERT_EQ(true, b.square_under_attack(Square("d7")));
    ASSERT_EQ(true, b.square_under_attack(Square("d8")));
    ASSERT_EQ(true, b.square_under_attack(Square("c4")));
    ASSERT_EQ(true, b.square_under_attack(Square("e4")));
    ASSERT_EQ(true, b.square_under_attack(Square("b4")));
    ASSERT_EQ(false, b.square_under_attack(Square("a4")));
    ASSERT_EQ(false, b.square_under_attack(Square("f4")));

    Board c = Board("7k/2p5/1b6/8/8/6B1/5P2/7K w - - 0 100");
    ASSERT_EQ(true, c.square_under_attack(Square("h2")));
    ASSERT_EQ(true, c.square_under_attack(Square("f2")));
    ASSERT_EQ(false, c.square_under_attack(Square("e1")));
    ASSERT_EQ(true, c.square_under_attack(Square("h4")));
    ASSERT_EQ(true, c.square_under_attack(Square("f4")));
    ASSERT_EQ(true, c.square_under_attack(Square("e5")));
    ASSERT_EQ(true, c.square_under_attack(Square("d6")));
    ASSERT_EQ(true, c.square_under_attack(Square("c7")));
    ASSERT_EQ(false, c.square_under_attack(Square("b8")));
    ASSERT_EQ(false, c.square_under_attack(Square("f3")));
    c.apply_ply(Ply("h1h2"));
    ASSERT_EQ(true, c.square_under_attack(Square("a7")));
    ASSERT_EQ(true, c.square_under_attack(Square("a5")));
    ASSERT_EQ(true, c.square_under_attack(Square("c7")));
    ASSERT_EQ(false, c.square_under_attack(Square("d8")));
    ASSERT_EQ(true, c.square_under_attack(Square("c5")));
    ASSERT_EQ(true, c.square_under_attack(Square("d4")));
    ASSERT_EQ(true, c.square_under_attack(Square("e3")));
    ASSERT_EQ(true, c.square_under_attack(Square("f2")));
    ASSERT_EQ(false, c.square_under_attack(Square("g2")));
    ASSERT_EQ(false, c.square_under_attack(Square("c6")));

    Board d = Board("7k/8/8/4p3/4P3/8/8/7K w - - 0 100");
    ASSERT_EQ(true, d.square_under_attack(Square("d5")));
    ASSERT_EQ(true, d.square_under_attack(Square("f5")));
    ASSERT_EQ(false, d.square_under_attack(Square("e5")));
    d.apply_ply(Ply("h1h2"));
    ASSERT_EQ(true, d.square_under_attack(Square("d4")));
    ASSERT_EQ(true, d.square_under_attack(Square("f4")));
    ASSERT_EQ(false, d.square_under_attack(Square("e4")));

    Board e = Board("8/3k4/8/8/8/8/3K4/8 w - - 0 100");
    ASSERT_EQ(true, e.square_under_attack(Square("c1")));
    ASSERT_EQ(true, e.square_under_attack(Square("d1")));
    ASSERT_EQ(true, e.square_under_attack(Square("e1")));
    ASSERT_EQ(true, e.square_under_attack(Square("e2")));
    ASSERT_EQ(true, e.square_under_attack(Square("e3")));
    ASSERT_EQ(true, e.square_under_attack(Square("d3")));
    ASSERT_EQ(true, e.square_under_attack(Square("c3")));
    ASSERT_EQ(true, e.square_under_attack(Square("c2")));
    ASSERT_EQ(false, e.square_under_attack(Square("d4")));
    e.apply_ply(Ply("e2d2"));
    ASSERT_EQ(true, e.square_under_attack(Square("c8")));
    ASSERT_EQ(true, e.square_under_attack(Square("d8")));
    ASSERT_EQ(true, e.square_under_attack(Square("e8")));
    ASSERT_EQ(true, e.square_under_attack(Square("e7")));
    ASSERT_EQ(true, e.square_under_attack(Square("e6")));
    ASSERT_EQ(true, e.square_under_attack(Square("d6")));
    ASSERT_EQ(true, e.square_under_attack(Square("c6")));
    ASSERT_EQ(true, e.square_under_attack(Square("c7")));
    ASSERT_EQ(false, e.square_under_attack(Square("d5")));
}

TEST(BoardTest, is_white_king_in_check) {
    Board a = Board("7k/8/8/8/8/8/6rq/7K w - - 0 100");
    ASSERT_EQ(true, a.is_white_king_in_check());

    Board b = Board("8/8/8/8/8/7k/6r1/7K w - - 0 100");
    ASSERT_EQ(false, b.is_white_king_in_check());
}

TEST(BoardTest, is_black_king_in_check) {
    Board a = Board("7k/6RQ/8/8/8/8/8/7K b - - 0 100");
    ASSERT_EQ(true, a.is_black_king_in_check());

    Board b = Board("7k/6R1/7K/8/8/8/8/8 b - - 0 100");
    ASSERT_EQ(false, b.is_black_king_in_check());
}

TEST(BoardTest, generate_potential_plies) {
    Board a = Board("8/7p/7P/1k6/8/1K6/8/8 w - - 0 100");
    std::vector<Ply> expected = { Ply("b3c3"), Ply("b3c2"), Ply("b3b2"), Ply("b3a2"), Ply("b3a3") };
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

    Board e = Board("7k/8/7p/6pP/1p6/8/P7/7K w - g6 0 100");
    expected = { Ply("h1h2"), Ply("h1g2"), Ply("h1g1"), Ply("h5g6"), Ply("a2a3"), Ply("a2a4") };
    std::sort(expected.begin(), expected.end());
    actual = e.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
    e.apply_ply(Ply("a2a4"));
    expected = { Ply("h8h7"), Ply("h8g7"), Ply("h8g8"), Ply("g5g4"), Ply("b4b3"), Ply("b4a3") };
    std::sort(expected.begin(), expected.end());
    actual = e.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
    e.apply_ply(Ply("h8h7"));
    expected = { Ply("h1h2"), Ply("h1g2"), Ply("h1g1"), Ply("a4a5") };
    std::sort(expected.begin(), expected.end());
    actual = e.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);

    Board f = Board("7k/8/8/7p/8/6N1/8/7K w - - 0 100");
    expected = { Ply("h1h2"), Ply("h1g2"), Ply("h1g1"), Ply("g3f1"), Ply("g3e2"), Ply("g3e4"), Ply("g3f5"), Ply("g3h5")};
    std::sort(expected.begin(), expected.end());
    actual = f.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);

    Board g = Board("1k6/8/8/8/8/1r6/8/1R4K1 w - - 0 100");
    expected = { Ply("g1f1"), Ply("g1f2"), Ply("g1g2"), Ply("g1h2"), Ply("g1h1"),
                 Ply("b1a1"), Ply("b1c1"), Ply("b1d1"), Ply("b1e1"), Ply("b1f1"),
                 Ply("b1b2"), Ply("b1b3")};
    std::sort(expected.begin(), expected.end());
    actual = g.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
    g.apply_ply(Ply("b1f1"));
    expected = { Ply("b8a8"), Ply("b8a7"), Ply("b8b7"), Ply("b8c7"), Ply("b8c8"),
                 Ply("b3b2"), Ply("b3b1"), Ply("b3b4"), Ply("b3b5"), Ply("b3b6"), Ply("b3b7"),
                 Ply("b3a3"), Ply("b3c3"), Ply("b3d3"), Ply("b3e3"), Ply("b3f3"), Ply("b3g3"), Ply("b3h3")};
    std::sort(expected.begin(), expected.end());
    actual = g.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);

    Board h = Board("7k/1p6/1Pq5/8/8/5Bp1/6P1/K7 w - - 0 100");
    expected = { Ply("a1b1"), Ply("a1b2"), Ply("a1a2"),
                 Ply("f3e4"), Ply("f3d5"), Ply("f3c6"),
                 Ply("f3g4"), Ply("f3h5"), Ply("f3e2"), Ply("f3d1") };
    std::sort(expected.begin(), expected.end());
    actual = h.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
    h.apply_ply(Ply("h1g1"));

    expected = { Ply("h8g8"), Ply("h8g7"), Ply("h8h7"),
                 Ply("c6d5"), Ply("c6e4"), Ply("c6f3"),
                 Ply("c6d7"), Ply("c6e8"), Ply("c6b5"), Ply("c6a4"),
                 Ply("c6c7"), Ply("c6c8"), Ply("c6c5"), Ply("c6c4"), Ply("c6c3"), Ply("c6c2"), Ply("c6c1"),
                 Ply("c6b6"), Ply("c6d6"), Ply("c6e6"), Ply("c6f6"), Ply("c6g6"), Ply("c6h6") };
    std::sort(expected.begin(), expected.end());
    actual = h.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);

    Board i = Board("7k/8/8/8/8/8/r7/7K w - - 0 100");
    expected = { Ply("h1g1") };
    actual = i.generate_potential_plies();
    ASSERT_EQ(expected, actual);

    Board j = Board("7k/7p/6PR/8/8/8/8/7K b - - 0 100");
    expected = { Ply("h8g8"), Ply("h8g7") };
    std::sort(expected.begin(), expected.end());
    actual = j.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);

    // All castling is legal
    Board k = Board("r3k2r/p6p/8/8/8/8/P6P/R3K2R w KQkq - 0 100");
    expected = { Ply("a1b1"), Ply("a1c1"), Ply("a1d1"), Ply("h1g1"), Ply("h1f1"),
                 Ply("a2a3"), Ply("a2a4"), Ply("h2h3"), Ply("h2h4"),
                 Ply("e1d1"), Ply("e1d2"), Ply("e1e2"), Ply("e1f2"), Ply("e1f1"),
                 Ply("e1c1"), Ply("e1g1") };
    std::sort(expected.begin(), expected.end());
    actual = k.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
    k.apply_ply(Ply("a2a3"));
    expected = { Ply("a8b8"), Ply("a8c8"), Ply("a8d8"), Ply("h8g8"), Ply("h8f8"),
                 Ply("a7a6"), Ply("a7a5"), Ply("h7h6"), Ply("h7h5"),
                 Ply("e8d8"), Ply("e8d7"), Ply("e8e7"), Ply("e8f7"), Ply("e8f8"),
                 Ply("e8c8"), Ply("e8g8") };
    std::sort(expected.begin(), expected.end());
    actual = k.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);

    // Square under attack prevents castling kingside
    Board l = Board("1r2k1r1/p6p/8/8/8/8/P6P/R3K2R w KQ - 0 100");
    expected = { Ply("a1b1"), Ply("a1c1"), Ply("a1d1"), Ply("h1g1"), Ply("h1f1"),
                 Ply("a2a3"), Ply("a2a4"), Ply("h2h3"), Ply("h2h4"),
                 Ply("e1d1"), Ply("e1d2"), Ply("e1e2"), Ply("e1f2"), Ply("e1f1"),
                 Ply("e1c1") };
    std::sort(expected.begin(), expected.end());
    actual = l.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);

    // Some castling is not legal because peices moved
    Board m = Board("r3k2r/p6p/8/8/8/8/P6P/R3K2R w Kq - 0 100");
    expected = { Ply("a1b1"), Ply("a1c1"), Ply("a1d1"), Ply("h1g1"), Ply("h1f1"),
                 Ply("a2a3"), Ply("a2a4"), Ply("h2h3"), Ply("h2h4"),
                 Ply("e1d1"), Ply("e1d2"), Ply("e1e2"), Ply("e1f2"), Ply("e1f1"),
                 Ply("e1g1") };
    std::sort(expected.begin(), expected.end());
    actual = m.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
    m.apply_ply(Ply("a2a3"));
    expected = { Ply("a8b8"), Ply("a8c8"), Ply("a8d8"), Ply("h8g8"), Ply("h8f8"),
                 Ply("a7a6"), Ply("a7a5"), Ply("h7h6"), Ply("h7h5"),
                 Ply("e8d8"), Ply("e8d7"), Ply("e8e7"), Ply("e8f7"), Ply("e8f8"),
                 Ply("e8c8") };
    std::sort(expected.begin(), expected.end());
    actual = m.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);

    // Pieces in the way prevent castling
    Board n = Board("rn2k2r/p6p/8/8/8/8/P6P/R3K1NR w KQkq - 0 100");
    expected = { Ply("a1b1"), Ply("a1c1"), Ply("a1d1"),
                 Ply("a2a3"), Ply("a2a4"), Ply("h2h3"), Ply("h2h4"),
                 Ply("g1h3"), Ply("g1f3"), Ply("g1e2"),
                 Ply("e1d1"), Ply("e1d2"), Ply("e1e2"), Ply("e1f2"), Ply("e1f1"),
                 Ply("e1c1") };
    std::sort(expected.begin(), expected.end());
    actual = n.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
    n.apply_ply(Ply("a2a3"));
    expected = { Ply("h8g8"), Ply("h8f8"),
                 Ply("a7a6"), Ply("a7a5"), Ply("h7h6"), Ply("h7h5"),
                 Ply("b8a6"), Ply("b8c6"), Ply("b8d7"),
                 Ply("e8d8"), Ply("e8d7"), Ply("e8e7"), Ply("e8f7"), Ply("e8f8"),
                 Ply("e8g8") };
    std::sort(expected.begin(), expected.end());
    actual = n.generate_potential_plies();
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
}

TEST(BoardTest, is_threefold_repetition) {
    Board a = Board("7k/6p1/8/8/8/8/8/7K b - - 0 100");
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("h8f8"));
    a.apply_ply(Ply("h1f1"));
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("f8h8"));
    a.apply_ply(Ply("f1h1"));
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("h8f8"));
    a.apply_ply(Ply("h1f1"));
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("f8h8"));
    a.apply_ply(Ply("f1h1"));
    ASSERT_EQ(true, a.is_threefold_repetition());

    // Tests with pawn push
    a = Board("7k/6p1/8/8/8/8/8/7K b - - 0 100");
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("h8f8"));
    a.apply_ply(Ply("h1f1"));
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("f8h8"));
    a.apply_ply(Ply("f1h1"));
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("h8f8"));
    a.apply_ply(Ply("h1f1"));
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("f7f5"));
    a.apply_ply(Ply("f1h2"));
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("f8h8"));
    a.apply_ply(Ply("h2h1"));
    ASSERT_EQ(false, a.is_threefold_repetition());

    // Tests turns
    a = Board("7k/6p1/8/8/8/8/8/7K b - - 0 100");
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("h8f8"));
    a.apply_ply(Ply("h1f1"));
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("f8h8"));
    a.apply_ply(Ply("f1h1"));
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("h8f8"));
    a.apply_ply(Ply("h1f1"));
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("f8h7"));
    a.apply_ply(Ply("f1h1"));
    ASSERT_EQ(false, a.is_threefold_repetition());
    a.apply_ply(Ply("h7h8"));
    ASSERT_EQ("7k/6p1/8/8/8/8/8/7K w - - 9 105", a.to_fen());
    ASSERT_EQ(false, a.is_threefold_repetition());
}

TEST(BoardTest, is_fifty_move_draw) {
    Board b = Board("7k/7r/8/8/8/8/7R/7K w - - 99 105");
    ASSERT_EQ(false, b.is_fifty_move_draw());
    b.apply_ply(Ply("h1f1"));
    ASSERT_EQ(true, b.is_fifty_move_draw());

    b = Board("7k/7r/8/8/8/8/7R/7K b - - 99 105");
    ASSERT_EQ(false, b.is_fifty_move_draw());
    b.apply_ply(Ply("h8f8"));
    ASSERT_EQ(true, b.is_fifty_move_draw());
}

TEST(BoardTest, is_insufficient_mating_material) {
    Board b = Board("7k/8/8/8/8/8/7r/7K w - - - 100");
    ASSERT_EQ(false, b.is_insufficient_mating_material());
    b.apply_ply(Ply("h1h2"));
    ASSERT_EQ(true, b.is_insufficient_mating_material());

    b = Board("7k/7R/8/8/8/8/8/7K b - - - 100");
    ASSERT_EQ(false, b.is_insufficient_mating_material());
    b.apply_ply(Ply("h8h7"));
    ASSERT_EQ(true, b.is_insufficient_mating_material());

    b = Board("7k/8/8/8/8/8/7p/7K w - - - 100");
    ASSERT_EQ(false, b.is_insufficient_mating_material());
    b.apply_ply(Ply("h1h2"));
    ASSERT_EQ(true, b.is_insufficient_mating_material());

    b = Board("7k/8/8/8/8/8/b6r/7K w - - - 100");
    ASSERT_EQ(false, b.is_insufficient_mating_material());
    b.apply_ply(Ply("h1h2"));
    ASSERT_EQ(true, b.is_insufficient_mating_material());

    b = Board("7k/8/8/8/8/8/1b5r/7K w - - - 100");
    ASSERT_EQ(false, b.is_insufficient_mating_material());
    b.apply_ply(Ply("h1h2"));
    ASSERT_EQ(true, b.is_insufficient_mating_material());

    b = Board("7k/8/8/8/8/8/b1b4r/7K w - - - 100");
    ASSERT_EQ(false, b.is_insufficient_mating_material());
    b.apply_ply(Ply("h1h2"));
    ASSERT_EQ(true, b.is_insufficient_mating_material());

    b = Board("7k/8/8/8/8/8/1b1b3r/7K w - - - 100");
    ASSERT_EQ(false, b.is_insufficient_mating_material());
    b.apply_ply(Ply("h1h2"));
    ASSERT_EQ(true, b.is_insufficient_mating_material());

    b = Board("7k/8/8/8/8/8/1b1B3r/7K w - - - 100");
    ASSERT_EQ(false, b.is_insufficient_mating_material());
    b.apply_ply(Ply("h1h2"));
    ASSERT_EQ(true, b.is_insufficient_mating_material());

    b = Board("7k/8/8/8/8/8/1b1B3p/7K w - - - 100");
    ASSERT_EQ(false, b.is_insufficient_mating_material());
    b.apply_ply(Ply("h1h2"));
    ASSERT_EQ(true, b.is_insufficient_mating_material());
}

TEST(BoardTest, legal_plies_integration) {
    for (auto& p : std::__fs::filesystem::directory_iterator("../src/test/fixtures/games/")) {
        std::ifstream game (p.path());

        Board b = Board::default_board();

        for (std::string line; std::getline(game, line); ) {
            std::vector<Ply> plies = b.generate_potential_plies();
            Ply new_ply = Ply(line);
            ASSERT_EQ(1, std::count(plies.begin(), plies.end(), new_ply));
            b.apply_ply(new_ply);
        }
    }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}