#pragma once

#include <cstdint>
#include <string>

class Square {

    public:
        constexpr Square() {}

        constexpr Square(std::uint8_t s) : _square(s) {};

        constexpr Square(const int row, const int col) : _square(row * 8 + col) {};

        Square(std::string s) {
            _square = (s[1] - '1') * 8 + (s[0] - 'a');
        };

        constexpr int get_row() const {
            return _square / 8;
        }
 
        constexpr int get_col() const {
            return _square % 8;
        }

        constexpr int get_int_value() const {
            return _square;
        }

        constexpr Square mirror() const {
            return Square(((7 - (_square / 8)) * 8) + (_square % 8));
        }

        constexpr bool operator==(const Square& other) const {
            return _square == other._square;
        }

        constexpr bool operator!=(const Square& other) const {
            return _square != other._square;
        }

        std::string to_string() const {
            std::string result;
            result += 'a' + get_col();
            result += '1' + get_row();
            return result;
        }

        std::uint8_t get_int() const { return _square; }

    private:
        std::uint8_t _square = 0;
};