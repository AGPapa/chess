#pragma once

#include <cstdint>

class Castling {

    public:
        Castling() {};

        Castling(bool white_kingside, bool white_queenside,
                bool black_kingside, bool black_queenside) {
            castling += white_kingside;
            castling += white_queenside << 1;
            castling += black_kingside << 2;
            castling += black_queenside << 3;
        };

        bool get_white_kingside() const { return castling & 1; }
        bool get_white_queenside() const { return castling & 2; }
        bool get_black_kingside() const { return castling & 4; }
        bool get_black_queenside() const { return castling & 8; }
        bool no_castle_legal() const { return castling == 0; }

        void unset_white_kingside() { castling &= ~std::uint64_t(1); }
        void unset_white_queenside() { castling &= ~std::uint64_t(2); }
        void unset_black_kingside() { castling &= ~std::uint64_t(4); }
        void unset_black_queenside() { castling &= ~std::uint64_t(8); }

        void unset_white() { castling &= ~std::uint64_t(3); }
        void unset_black() { castling &= ~std::uint64_t(12); }

        bool operator==(const Castling& other) const { return castling == other.castling; }
        bool operator!=(const Castling& other) const { return castling != other.castling; }


    private:
        // - Bit 0 -- white kingside castle.
        // - Bit 1 -- white queenside castle.
        // - Bit 2 -- black's kingside castle.
        // - Bit 3 -- black's queenside castle.
        std::uint8_t castling = 0;
};