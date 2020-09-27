 #include <cstdint>
 #include <string>
 #include <iostream>
 #include "bitboard.cpp"


class Board {

    public:
        Board() {};
        
        Board(Bitboard p, Bitboard r, Bitboard n, Bitboard b, Square k) {
            pawns = p;
            rooks = r;
            knights = n;
            bishops = b;
            king = k;
        };

        static Board default_board() {
            Bitboard pawns = Bitboard(0x00FF000000000000);
            Bitboard rooks = Bitboard(0x8900000000000000);
            Bitboard knights = Bitboard(0x4200000000000000);
            Bitboard bishops = Bitboard(0x2C00000000000000);
            Square king = Square(0x3C);
            return Board(pawns, rooks, knights, bishops, king);
        }

        std::string to_string() {
            std::string result;
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    if (row == king.get_row() && col == king.get_col()) {
                        result += 'K';
                    } else if (rooks.get_square(row, col) && bishops.get_square(row, col)) {
                        result += 'Q';
                    } else if (pawns.get_square(row, col)) {
                        result += 'P';
                    } else if (rooks.get_square(row, col)) {
                        result += 'R';
                    } else if (knights.get_square(row, col)) {
                        result += 'N';
                    } else if (bishops.get_square(row, col)) {
                        result += 'B';
                    } else {
                        result += '.';
                    }
                }
                result += '\n';
            }
            return result;
        }
    
    private:
        Bitboard pawns;
        Bitboard rooks; // includes Queens
        Bitboard knights;
        Bitboard bishops; // includes Queens
        Square king;
};

int main(int argc, char** argv) {
    Board b = Board::default_board();
    std::cout << b.to_string();
  return 0;
}