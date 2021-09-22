#pragma once

#include <cstdint>

class Castling {

    public:
        Castling() {};

        Castling(std::uint8_t castling) {
            _castling = castling;
        }

        Castling(bool white_kingside, bool white_queenside,
                bool black_kingside, bool black_queenside) {
            _castling += white_kingside;
            _castling += white_queenside << 1;
            _castling += black_kingside << 2;
            _castling += black_queenside << 3;
        };

        bool get_white_kingside() const { return _castling & 1; }
        bool get_white_queenside() const { return _castling & 2; }
        bool get_black_kingside() const { return _castling & 4; }
        bool get_black_queenside() const { return _castling & 8; }
        bool no_castle_legal() const { return _castling == 0; }

        void unset_white_kingside() { _castling &= ~std::uint64_t(1); }
        void unset_white_queenside() { _castling &= ~std::uint64_t(2); }
        void unset_black_kingside() { _castling &= ~std::uint64_t(4); }
        void unset_black_queenside() { _castling &= ~std::uint64_t(8); }

        void unset_white() { _castling &= ~std::uint64_t(3); }
        void unset_black() { _castling &= ~std::uint64_t(12); }

        Castling mirror() { return Castling((_castling >> 2) + ((_castling & 0b00000011) << 2)); }

        bool operator==(const Castling& other) const { return _castling == other._castling; }
        bool operator!=(const Castling& other) const { return _castling != other._castling; }


    private:
        // - Bit 0 -- white kingside castle.
        // - Bit 1 -- white queenside castle.
        // - Bit 2 -- black's kingside castle.
        // - Bit 3 -- black's queenside castle.
        std::uint8_t _castling = 0;
};