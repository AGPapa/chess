#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include "square.cpp"

class Bitboard {

    public:
        Bitboard() {
            board = 0;
        };

        Bitboard(std::uint64_t b) {
            board = b;
        };

        void set_square(int row, int col) {
            board |= (std::uint64_t(1) << (row*8 + col));
        }

        void set_square(const Square s) {
             board |= (std::uint64_t(1) << s.get_int_value());
        }

        void set_square_if(int row, int col, bool condition) {
            board |= (std::uint64_t(condition) << (row*8 + col));
        }

        void set_square_if(const Square s, bool condition) {
            board |= (std::uint64_t(condition) << s.get_int_value());
        }

        void unset_square(int row, int col) {
            board &= ~(std::uint64_t(1) << (row*8 + col));
        }

        void unset_square(const Square s) {
             board &= ~(std::uint64_t(1) << s.get_int_value());
        }

        int get_square(int row, int col) const {
            return (board >> (row*8+col)) & 1;
        }

        int get_square(int index) const {
            return (board >> index) & 1;
        }

        int get_square(const Square s) const {
            return (board >> s.get_int_value()) & 1;
        }

        Bitboard mirror() const {
            std::uint64_t new_board;
            new_board = (board & 0x00000000FFFFFFFF) << 32 | (board & 0xFFFFFFFF00000000) >> 32;
            new_board = (new_board & 0x0000FFFF0000FFFF) << 16 | (new_board & 0xFFFF0000FFFF0000) >> 16;
            new_board = (new_board & 0x00FF00FF00FF00FF) << 8 | (new_board & 0xFF00FF00FF00FF00) >> 8;
            return Bitboard(new_board);
        }

        bool empty() const {
            return board == 0;
        }

        int count() const {
            return __builtin_popcountll(board);
        }

        Bitboard squarewise_not() const {
            return ~board;
        }

        friend Bitboard squarewise_or(const Bitboard a, const Bitboard b) {
            return Bitboard(a.board | b.board);
        }

        friend Bitboard squarewise_and(const Bitboard a, const Bitboard b) {
            return Bitboard(a.board & b.board);
        }

        bool operator==(const Bitboard& other) const {
            return board == other.board;
        }

        bool operator!=(const Bitboard& other) const {
            return board != other.board;
        }

        std::string to_string() {
            std::string result;
            for (int row = 7; row >= 0; row--) {
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
