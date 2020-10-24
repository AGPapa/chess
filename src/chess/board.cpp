 #include <cstdint>
 #include <string>
 #include <iostream>
 #include "bitboard.cpp"
 #include "castling.cpp"


class Board {

    public:
        Board() {};
        
        Board(Bitboard w_p, Bitboard w_r, Bitboard w_n, Bitboard w_b, Square w_k,
              Bitboard b_p, Bitboard b_r, Bitboard b_n, Bitboard b_b, Square b_k,
              bool w_t, Castling c) {
            w_pawns = w_p;
            w_rooks = w_r;
            w_knights = w_n;
            w_bishops = w_b;
            w_king = w_k;

            b_pawns = b_p;
            b_rooks = b_r;
            b_knights = b_n;
            b_bishops = b_b;
            b_king = b_k;

            w_turn = w_t;
            castling = c;
        };

        static Board default_board() {
            Bitboard w_pawns = Bitboard(0x0000000000FF00);
            Bitboard w_rooks = Bitboard(0x0000000000000089);
            Bitboard w_knights = Bitboard(0x0000000000000042);
            Bitboard w_bishops = Bitboard(0x000000000000002C);
            Square w_king = Square(0x04);

            Bitboard b_pawns = Bitboard(0x00FF000000000000);
            Bitboard b_rooks = Bitboard(0x8900000000000000);
            Bitboard b_knights = Bitboard(0x4200000000000000);
            Bitboard b_bishops = Bitboard(0x2C00000000000000);
            Square b_king = Square(0x3C);

            Castling c = Castling(true, true, true, true);

            return Board(w_pawns, w_rooks, w_knights, w_bishops, w_king,
                         b_pawns, b_rooks, b_knights, b_bishops, b_king,
                         true, c);
        }

        Bitboard all_pieces() {
            Bitboard b = squarewise_or(
                squarewise_or(
                    squarewise_or(w_pawns, b_pawns),
                    squarewise_or(w_rooks, b_rooks)
                ),
                squarewise_or(
                    squarewise_or(w_bishops, b_bishops),
                    squarewise_or(w_knights, b_knights)
                )
            );
            b.set_square(w_king);
            b.set_square(b_king);
            return b;
        }

        std::string to_fen() {
            // TODO: Add 50 move rule, castling, en-passant
            std::string result;
            int empty = 0;
            for (int row = 7; row >= 0; row--) {
                for (int col = 0; col < 8; col++) {
                    if (all_pieces().get_square(row, col) == 0) {
                        empty++;
                    } else {
                        if (empty != 0) {
                            result += empty + 48;
                        }
                        empty = 0;
                        result += square_to_char(row, col);
                    }
                }
                if (empty != 0) {
                    result += empty + 48;
                }
                empty = 0;
                if (row != 0) {
                    result += '/';
                }
            }
            result += w_turn ? " w " : " b ";
            if (castling.no_castle_legal()) {
                result += '-';
            } else {
                if (castling.get_white_kingside()) { result += "K"; };
                if (castling.get_white_queenside()) { result += "Q"; };
                if (castling.get_black_kingside()) { result += "k"; };
                if (castling.get_black_queenside()) { result += "q"; };
            }
            result += '\n';
            return result;
        }

        std::string to_string() {
            std::string result;
            for (int row = 7; row >= 0; row--) {
                for (int col = 0; col < 8; col++) {
                    result += square_to_char(row, col);
                }
                result += '\n';
            }
            return result;
        }
    
    private:
        Bitboard w_pawns;
        Bitboard w_rooks; // includes Queens
        Bitboard w_knights;
        Bitboard w_bishops; // includes Queens
        Square w_king;

        Bitboard b_pawns;
        Bitboard b_rooks; // includes Queens
        Bitboard b_knights;
        Bitboard b_bishops; // includes Queens
        Square b_king;

        bool w_turn;

        Castling castling;

        char square_to_char(int row, int col) {
            if (row == w_king.get_row() && col == w_king.get_col()) {
                return 'K';
            } else if (w_rooks.get_square(row, col) && w_bishops.get_square(row, col)) {
                return 'Q';
            } else if (w_pawns.get_square(row, col)) {
                return 'P';
            } else if (w_rooks.get_square(row, col)) {
                return 'R';
            } else if (w_knights.get_square(row, col)) {
                return 'N';
            } else if (w_bishops.get_square(row, col)) {
                return 'B';
            } else if (row == b_king.get_row() && col == b_king.get_col()) {
                return 'k';
            } else if (b_rooks.get_square(row, col) && b_bishops.get_square(row, col)) {
                return 'q';
            } else if (b_pawns.get_square(row, col)) {
                return 'p';
            } else if (b_rooks.get_square(row, col)) {
                return 'r';
            } else if (b_knights.get_square(row, col)) {
                return 'n';
            } else if (b_bishops.get_square(row, col)) {
                return 'b';
            } else {
                return '.';
            }
        }
};

int main(int argc, char** argv) {
    Board b = Board::default_board();
    std::cout << b.to_string() << b.to_fen();
  return 0;
}