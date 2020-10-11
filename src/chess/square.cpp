#include <cstdint>
#include <string>

class Square {

    public:
        Square() {}

        Square(const int row, const int col) {
            square = row * 8 + col;
        };

        Square(std::uint8_t s) {
            square = s;
        };

        Square(std::string s) {
            square = (s[1] - '1') * 8 + (s[0] - 'a');
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

        std::string to_string() const {
            std::string result;
            result += 'a' + get_col();
            result += '1' + get_row();
            return result;
        }

    private:
        std::uint8_t square = 0;
};