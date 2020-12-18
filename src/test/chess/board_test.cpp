#include "../../chess/board.cpp"

#include <gtest/gtest.h>

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

    Board c = Board("4k2r/6r1/8/8/8/8/3R4/R3K3 w Qk - 0 1");
    ASSERT_EQ("4k2r/6r1/8/8/8/8/3R4/R3K3 w Qk - 0 1", c.to_fen());
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
    ASSERT_EQ("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1", a.to_fen());

    a.apply_ply(Ply("d7d5"));
    ASSERT_EQ("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2", a.to_fen());

    a.apply_ply(Ply("e4d5"));
    ASSERT_EQ("rnbqkbnr/ppp1pppp/8/3P4/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 2", a.to_fen());

    a.apply_ply(Ply("d8d5"));
    ASSERT_EQ("rnb1kbnr/ppp1pppp/8/3q4/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 3", a.to_fen());

    a.apply_ply(Ply("e1e2"));
    ASSERT_EQ("rnb1kbnr/ppp1pppp/8/3q4/8/8/PPPPKPPP/RNBQ1BNR b kq - 1 3", a.to_fen());

    a.apply_ply(Ply("e8d8"));
    ASSERT_EQ("rnbk1bnr/ppp1pppp/8/3q4/8/8/PPPPKPPP/RNBQ1BNR w - - 2 4", a.to_fen());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}