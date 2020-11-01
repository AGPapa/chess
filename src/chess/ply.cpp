#include <cstdint>
#include <string>
#include "square.cpp"

class Ply {
    public:
        Ply() {};

        Ply(const Square from, const Square to) {
            ply = (to.get_int_value() + (from.get_int_value() << 6));
        }

        Ply(const std::string& str) {
            Square from = str.substr(0, 2);
            Square to = str.substr(2, 2);
            ply = (to.get_int_value() + (from.get_int_value() << 6));
        }

        Square from_square() const {
            return Square((ply & from_mask) >> 6);
        }

        Square to_square() const {
            return Square(ply & to_mask);
        }

        std::string to_string() const {
            std::string result;
            result += from_square().to_string();
            result += to_square().to_string();
            return result;
        }

    private:
        // Bits 0 to 5 -- to
        // Bits 6 to 11 -- from
        std::uint16_t ply = 0;

        const std::uint16_t to_mask = 0b0000000000111111;
        const std::uint16_t from_mask = 0b0000111111000000;
};