 #include <cstdint>

class Square {

    public:
        Square() {}

        Square(int row, int col) {
            square = row * 8 + col;
        };

        Square(std::uint8_t s) {
            square = s;
        };

        int get_row() const {
            return square / 8;
        }
 
        int get_col() const {
            return square % 8;
        }

        int get_int_value() const {
            return square;
        }

    private:
        std::uint8_t square = 0;
};