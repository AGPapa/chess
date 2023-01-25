#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include "square.cpp"

class Bitboard {

    public:
        Bitboard() {
            _board = 0;
        };

        Bitboard(std::uint64_t b) {
            _board = b;
        };

        void set_square(int row, int col) {
            _board |= (std::uint64_t(1) << (row*8 + col));
        }

        void set_square(const Square s) {
             _board |= (std::uint64_t(1) << s.get_int_value());
        }

        void set_square_if(int row, int col, bool condition) {
            _board |= (std::uint64_t(condition) << (row*8 + col));
        }

        void set_square_if(const Square s, bool condition) {
            _board |= (std::uint64_t(condition) << s.get_int_value());
        }

        void unset_square(int row, int col) {
            _board &= ~(std::uint64_t(1) << (row*8 + col));
        }

        void unset_square(const Square s) {
             _board &= ~(std::uint64_t(1) << s.get_int_value());
        }

        int get_square(int row, int col) const {
            return (_board >> (row*8+col)) & 1;
        }

        int get_square(int index) const {
            return (_board >> index) & 1;
        }

        int get_square(const Square s) const {
            return (_board >> s.get_int_value()) & 1;
        }

        Bitboard mirror() const {
            std::uint64_t new_board;
            new_board = (_board & 0x00000000FFFFFFFF) << 32 | (_board & 0xFFFFFFFF00000000) >> 32;
            new_board = (new_board & 0x0000FFFF0000FFFF) << 16 | (new_board & 0xFFFF0000FFFF0000) >> 16;
            new_board = (new_board & 0x00FF00FF00FF00FF) << 8 | (new_board & 0xFF00FF00FF00FF00) >> 8;
            return Bitboard(new_board);
        }

        bool empty() const {
            return _board == 0;
        }

        int count() const {
            return __builtin_popcountll(_board);
        }

        Bitboard squarewise_not() const {
            return ~_board;
        }

        friend Bitboard squarewise_or(const Bitboard a, const Bitboard b) {
            return Bitboard(a._board | b._board);
        }

        friend Bitboard squarewise_and(const Bitboard a, const Bitboard b) {
            return Bitboard(a._board & b._board);
        }

        bool operator==(const Bitboard& other) const {
            return _board == other._board;
        }

        bool operator!=(const Bitboard& other) const {
            return _board != other._board;
        }

        std::string to_string() const {
            std::string result;
            for (int row = 7; row >= 0; row--) {
                for (int col = 0; col < 8; col++) {
                    result += ((_board >> (row*8+col)) & 1) + 48;
                }
                result += '\n';
            }
            return result;
        };

        std::uint64_t get_int() const {
            return _board;
        }

    private:
        std::uint64_t _board;
};
