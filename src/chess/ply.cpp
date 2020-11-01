#include <cstdint>
#include "square.cpp"

class Ply {
    public:
        Ply() {};

        Ply(Square from, Square to) {
            ply = (to.get_int_value() + (from.get_int_value() << 6));
        }

        Square from_square() {
            return Square((ply & from_mask) >> 6);
        }

        Square to_square() {
            return Square(ply & to_mask);
        }

    private:
        // Bits 0 to 5 -- to
        // Bits 6 to 11 -- from
        std::uint16_t ply = 0;

        const std::uint16_t to_mask = 0b0000000000111111;
        const std::uint16_t from_mask = 0b0000111111000000;
};