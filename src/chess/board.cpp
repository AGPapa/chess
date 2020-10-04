 #include <cstdint>
 #include <string>
 #include <iostream>
 #include "bitboard.cpp"


class Board {

    public:
        Board() {};
        
        Board(Bitboard w_p, Bitboard w_r, Bitboard w_n, Bitboard w_b, Square w_k,
              Bitboard b_p, Bitboard b_r, Bitboard b_n, Bitboard b_b, Square b_k) {
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

            return Board(w_pawns, w_rooks, w_knights, w_bishops, w_king,
                         b_pawns, b_rooks, b_knights, b_bishops, b_king);
        }

        std::string to_string() {
            std::string result;
            for (int row = 7; row >= 0; row--) {
                for (int col = 0; col < 8; col++) {
                    if (row == w_king.get_row() && col == w_king.get_col()) {
                        result += 'K';
                    } else if (w_rooks.get_square(row, col) && w_bishops.get_square(row, col)) {
                        result += 'Q';
                    } else if (w_pawns.get_square(row, col)) {
                        result += 'P';
                    } else if (w_rooks.get_square(row, col)) {
                        result += 'R';
                    } else if (w_knights.get_square(row, col)) {
                        result += 'N';
                    } else if (w_bishops.get_square(row, col)) {
                        result += 'B';
                    } else if (row == b_king.get_row() && col == b_king.get_col()) {
                        result += 'k';
                    } else if (b_rooks.get_square(row, col) && b_bishops.get_square(row, col)) {
                        result += 'q';
                    } else if (b_pawns.get_square(row, col)) {
                        result += 'p';
                    } else if (b_rooks.get_square(row, col)) {
                        result += 'r';
                    } else if (b_knights.get_square(row, col)) {
                        result += 'n';
                    } else if (b_bishops.get_square(row, col)) {
                        result += 'b';
                    } else {
                        result += '.';
                    }
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
};

int main(int argc, char** argv) {
    Board b = Board::default_board();
    std::cout << b.to_string();
  return 0;
}