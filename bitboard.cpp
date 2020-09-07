 #include <cstdint>
 #include <string>
 #include <iostream>
 
class Bitboard {
    public:

        Bitboard() {
            board = 0;
        };

        void set_bit(int row, int col) {
            board |= (std::uint64_t(1) << (row*8 + col));
        }

        int get_bit(int row, int col) {
            return (board >> (row*8+col)) & 1;
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
