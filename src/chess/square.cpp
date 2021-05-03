#pragma once

#include <cstdint>
#include <string>

class Square {

    public:
        constexpr Square() {}

        constexpr Square(std::uint8_t s) : square(s) {};

        constexpr Square(const int row, const int col) : square(row * 8 + col) {};

        Square(std::string s) {
            square = (s[1] - '1') * 8 + (s[0] - 'a');
        };

        constexpr int get_row() const {
            return square / 8;
        }
 
        constexpr int get_col() const {
            return square % 8;
        }

        constexpr int get_int_value() const {
            return square;
        }

        constexpr Square mirror() const {
            return Square(((7 - (square / 8)) * 8) + (square % 8));
        }

        constexpr bool operator==(const Square& other) const {
            return square == other.square;
        }

        constexpr bool operator!=(const Square& other) const {
            return square != other.square;
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