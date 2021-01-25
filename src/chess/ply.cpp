#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include "square.cpp"

class Ply {
    public:
        enum class MiscInfo : std::uint8_t { NoInfo, QueenPromote, RookPromote, BishopPromote, KnightPromote,
                                             PawnMove, KnightMove, BishopMove, RookMove, QueenMove, KingMove, Castling };

        Ply() {};

        Ply(const Square from, const Square to) {
            ply = (to.get_int_value() + (from.get_int_value() << 6));
        }

        Ply(const Square from, const Square to, const MiscInfo promotion) {
            ply = (to.get_int_value() + (from.get_int_value() << 6) + (static_cast<uint8_t>(promotion) << 12));
        }

        Ply(const std::string& str) {
            Square from = str.substr(0, 2);
            Square to = str.substr(2, 2);
            ply = (to.get_int_value() + (from.get_int_value() << 6));
            if (str.size() != 4) {
                if (str.size() != 5) throw std::runtime_error("Invalid ply - invalid length: " + str);
                switch (str[4]) {
                    case 'q':
                        ply += (static_cast<uint8_t>(MiscInfo::QueenPromote) << 12);
                        break;
                    case 'n':
                        ply += (static_cast<uint8_t>(MiscInfo::KnightPromote) << 12);
                        break;
                    case 'b':
                        ply += (static_cast<uint8_t>(MiscInfo::BishopPromote) << 12);
                        break;
                    case 'r':
                        ply += (static_cast<uint8_t>(MiscInfo::RookPromote) << 12);
                        break;
                    default:
                        throw std::runtime_error("Invalid ply - invalid promotion: " + str);
                }
            }
        }

        Square from_square() const {
            return Square((ply & from_mask) >> 6);
        }

        Square to_square() const {
            return Square(ply & to_mask);
        }

        MiscInfo promotion() const {
            return MiscInfo((ply & misc_info_mask) >> 12);
        }

        bool operator==(const Ply& other) const {
            return ply == other.ply;
        }

        bool operator!=(const Ply& other) const {
            return ply != other.ply;
        }

        bool operator<(const Ply& other) const {
            return ply < other.ply;
        }

        std::string to_string() const {
            std::string result;
            result += from_square().to_string();
            result += to_square().to_string();
            switch (promotion()) {
                case MiscInfo::QueenPromote:
                    return result + 'q';
                case MiscInfo::KnightPromote:
                    return result + 'n';
                case MiscInfo::BishopPromote:
                    return result + 'b';
                case MiscInfo::RookPromote:
                    return result + 'r';
                default:
                    return result;
            };
        }

    private:
        // Bits 0 to 5 -- to
        // Bits 6 to 11 -- from
        // Bits 12 to 14 -- promotion
        std::uint16_t ply = 0;

        static const std::uint16_t to_mask = 0b0000000000111111;
        static const std::uint16_t from_mask = 0b0000111111000000;
        static const std::uint16_t misc_info_mask = 0b0111000000000000;
};