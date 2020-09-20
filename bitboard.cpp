 #include <cstdint>
 #include <string>
 #include <iostream>
 
class Bitboard {

    public:

        Bitboard() {
            board = 0;
        };

        Bitboard(std::uint64_t b) {
            board = b;
        };

        void set_square(int row, int col) {
            board |= (std::uint64_t(1) << (row*8 + col));
        }

        int get_square(int row, int col) {
            return (board >> (row*8+col)) & 1;
        }

        friend Bitboard squarewise_or(const Bitboard a, const Bitboard b) {
            return Bitboard(a.board | b.board);
        }

        friend Bitboard squarewise_and(const Bitboard a, const Bitboard b) {
            return Bitboard(a.board & b.board);
        }

        std::string to_string() {
            std::string result;
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    result += ((board >> (row*8+col)) & 1) + 48;
                }
                result += '\n';
            }
            return result;
        };

    private:
        std::uint64_t board;
};
