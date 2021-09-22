#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include "square.cpp"

class Ply {
    public:
        enum class Promotion : std::uint8_t { None, Queen, Rook, Bishop, Knight };

        Ply() {};

        Ply(const Square from, const Square to) {
            ply = (to.get_int_value() + (from.get_int_value() << 6));
        }

        Ply(const Square from, const Square to, const Promotion promotion) {
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
                        ply += (static_cast<uint8_t>(Promotion::Queen) << 12);
                        break;
                    case 'n':
                        ply += (static_cast<uint8_t>(Promotion::Knight) << 12);
                        break;
                    case 'b':
                        ply += (static_cast<uint8_t>(Promotion::Bishop) << 12);
                        break;
                    case 'r':
                        ply += (static_cast<uint8_t>(Promotion::Rook) << 12);
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

        Promotion promotion() const {
            return Promotion((ply & promotion_mask) >> 12);
        }

        Ply mirror() const {
            return Ply(from_square().mirror(), to_square().mirror(), promotion());
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
                case Promotion::None:
                    return result;
                case Promotion::Queen:
                    return result + 'q';
                case Promotion::Knight:
                    return result + 'n';
                case Promotion::Bishop:
                    return result + 'b';
                case Promotion::Rook:
                    return result + 'r';
            };
        }

    private:
        // Bits 0 to 5 -- to
        // Bits 6 to 11 -- from
        // Bits 12 to 14 -- promotion
        std::uint16_t ply = 0;

        static const std::uint16_t to_mask = 0b0000000000111111;
        static const std::uint16_t from_mask = 0b0000111111000000;
        static const std::uint16_t promotion_mask = 0b0111000000000000;
};