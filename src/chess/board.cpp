#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "piece_attacks.hpp"
#include "bitboard.cpp"
#include "castling.cpp"
#include "ply.cpp"


class Board {

    public:
        Board() {};
        
        Board(Bitboard w_p, Bitboard w_r, Bitboard w_n, Bitboard w_b, Square w_k,
              Bitboard b_p, Bitboard b_r, Bitboard b_n, Bitboard b_b, Square b_k,
              bool w_t, Castling c, int r50_ply, int m_c) {
            w_pawns = w_p;
            w_rooks = w_r;
            w_knights = w_n;
            w_bishops = w_b;
            w_king = w_k;

            b_pawns = b_p;
            b_rooks = b_r;
            b_knights = b_n;
            b_bishops = b_b;
            b_king = b_k;

            w_turn = w_t;
            castling = c;

            rule_fifty_ply_clock = r50_ply;
            move_count = m_c;
        };

        Board(std::string fen) {
            std::istringstream fen_stream (fen);
            std::string peices;
            std::string turn;
            std::string castlings;
            std::string en_passant;

            fen_stream >> peices;

            int row = 7;
            int col = 0;
            for (char c : peices) {
                if (c == '/') {
                    --row;
                    if (row < 0) throw std::runtime_error("Bad fen string - too many rows: " + fen);
                    col = 0;
                    continue;
                }
                if (std::isdigit(c)) {
                    col += (c - '0');
                    continue;
                }
                if (col >= 8) throw std::runtime_error("Bad fen string - too many columns: " + fen);
            
                if (c == 'K') {
                    w_king = Square(row, col);
                } else if (c == 'Q') {
                    w_bishops.set_square(row, col);
                    w_rooks.set_square(row, col);
                } else if (c == 'P') {
                    w_pawns.set_square(row, col);
                } else if (c == 'R') {
                    w_rooks.set_square(row, col);
                } else if (c == 'N') {
                    w_knights.set_square(row, col);
                } else if (c == 'B') {
                    w_bishops.set_square(row, col);
                } else if (c == 'k') {
                    b_king = Square(row, col);
                } else if (c == 'q') {
                    b_bishops.set_square(row, col);
                    b_rooks.set_square(row, col);
                } else if (c == 'p') {
                    b_pawns.set_square(row, col);
                } else if (c == 'r') {
                    b_rooks.set_square(row, col);
                } else if (c == 'n') {
                    b_knights.set_square(row, col);
                } else if (c == 'b') {
                    b_bishops.set_square(row, col);
                } else {
                    throw std::runtime_error("Bad fen string - invalid piece char: " + fen);
                }
                col++;
            }

            fen_stream >> turn;
            if (turn == "w") { 
                w_turn = true;
            } else if (turn == "b") {
                w_turn = false;
            } else {
                throw std::runtime_error("Bad fen string - invalid turn: " + fen);
            }

            fen_stream >> castlings;
            if (castlings == "-") {
                castling = Castling(false, false, false, false);
            } else if (castlings == "K") {
                castling = Castling(true, false, false, false);
            } else if (castlings == "Q") {
                castling = Castling(false, true, false, false);
            } else if (castlings == "KQ") {
                castling = Castling(true, true, false, false);
            } else if (castlings == "k") {
                castling = Castling(false, false, true, false);
            } else if (castlings == "Kk") {
                castling = Castling(true, false, true, false);
            } else if (castlings == "Qk") {
                castling = Castling(false, true, true, false);
            } else if (castlings == "KQk") {
                castling = Castling(true, true, true, false);
            } else if (castlings == "q") {
                castling = Castling(false, false, false, true);
            } else if (castlings == "Kq") {
                castling = Castling(true, false, false, true);
            } else if (castlings == "Qq") {
                castling = Castling(false, true, false, true);
            } else if (castlings == "KQq") {
                castling = Castling(true, true, false, true);
            } else if (castlings == "kq") {
                castling = Castling(false, false, true, true);
            } else if (castlings == "Kkq") {
                castling = Castling(true, false, true, true);
            } else if (castlings == "Qkq") {
                castling = Castling(false, true, true, true);
            } else if (castlings == "KQkq") {
                castling = Castling(true, true, true, true);
            } else {
                throw std::runtime_error("Bad fen string - invalid castling: " + fen);
            }
           
            fen_stream >> en_passant;
            if (en_passant == "a3") {
                w_pawns.set_square(0, 0);
            } else if (en_passant == "b3") {
                w_pawns.set_square(0, 1);
            } else if (en_passant == "c3") {
                w_pawns.set_square(0, 2);
            } else if (en_passant == "d3") {
                w_pawns.set_square(0, 3);
            } else if (en_passant == "e3") {
                w_pawns.set_square(0, 4);
            } else if (en_passant == "f3") {
                w_pawns.set_square(0, 5);
            } else if (en_passant == "g3") {
                w_pawns.set_square(0, 6);
            } else if (en_passant == "h3") {
                w_pawns.set_square(0, 7);
            } else if (en_passant == "a6") {
                b_pawns.set_square(7, 0);
            } else if (en_passant == "b6") {
                b_pawns.set_square(7, 1);
            } else if (en_passant == "c6") {
                b_pawns.set_square(7, 2);
            } else if (en_passant == "d6") {
                b_pawns.set_square(7, 3);
            } else if (en_passant == "e6") {
                b_pawns.set_square(7, 4);
            } else if (en_passant == "f6") {
                b_pawns.set_square(7, 5);
            } else if (en_passant == "g6") {
                b_pawns.set_square(7, 6);
            } else if (en_passant == "h6") {
                b_pawns.set_square(7, 7);
            } else if (en_passant != "-") {
                throw std::runtime_error("Bad fen string - invalid en_passant: " + fen);
            }

            fen_stream >> rule_fifty_ply_clock;
            fen_stream >> move_count;
        }

        static Board default_board() {
            Bitboard w_pawns = Bitboard(0x0000000000FF00);
            Bitboard w_rooks = Bitboard(0x0000000000000089);
            Bitboard w_knights = Bitboard(0x0000000000000042);
            Bitboard w_bishops = Bitboard(0x000000000000002C);
            Square w_king = Square(0x04);

            Bitboard b_pawns = Bitboard(0x00FF000000000000);
            Bitboard b_rooks = Bitboard(0x8900000000000000);
            Bitboard b_knights = Bitboard(0x4200000000000000);
            Bitboard b_bishops = Bitboard(0x2C00000000000000);
            Square b_king = Square(0x3C);

            Castling c = Castling(true, true, true, true);

            return Board(w_pawns, w_rooks, w_knights, w_bishops, w_king,
                         b_pawns, b_rooks, b_knights, b_bishops, b_king,
                         true, c, 0, 1);
        }

        Bitboard white_pawns() const {
            return squarewise_and(w_pawns, pawn_mask);
        }

        Bitboard black_pawns() const {
            return squarewise_and(b_pawns, pawn_mask);
        }

        Bitboard all_pawns() const {
            return squarewise_and(squarewise_or(w_pawns, b_pawns), pawn_mask);
        }

        Bitboard white_queens() const {
            return squarewise_and(w_bishops, w_rooks);
        }

        Bitboard black_queens() const {
            return squarewise_and(b_bishops, b_rooks);
        }

        Bitboard white_en_passant() const {
            return squarewise_and(w_pawns, en_passant_mask);
        }

        Bitboard black_en_passant() const {
            return squarewise_and(b_pawns, en_passant_mask);
        }

        Bitboard all_en_passant() const {
            return squarewise_and(squarewise_or(w_pawns, b_pawns), en_passant_mask);
        }

        Bitboard all_pieces() const {
            Bitboard b = squarewise_or(
                squarewise_or(
                    all_pawns(),
                    squarewise_or(w_rooks, b_rooks)
                ),
                squarewise_or(
                    squarewise_or(w_bishops, b_bishops),
                    squarewise_or(w_knights, b_knights)
                )
            );
            b.set_square(w_king);
            b.set_square(b_king);
            return b;
        }

        Bitboard white_pieces() const {
            Bitboard w = squarewise_or(
                squarewise_or(squarewise_and(w_pawns, pawn_mask), w_rooks),
                squarewise_or(w_knights, w_bishops)
            );
            w.set_square(w_king);
            return w;
        }

        Bitboard black_pieces() const {
            Bitboard b = squarewise_or(
                squarewise_or(squarewise_and(b_pawns, pawn_mask), b_rooks),
                squarewise_or(b_knights, b_bishops)
            );
            b.set_square(b_king);
            return b;
        }

        void apply_ply(Ply ply) {
            Square from = ply.from_square();
            Square to = ply.to_square();

            bool reset_50;
            if (w_turn) {
                reset_50 = white_pawns().get_square(from);
            } else {
                reset_50 = black_pawns().get_square(from);
            }

            // move king
            if (from == w_king) {
                w_king = to;
                castling.unset_white();
                if (to == Square("g1") && from == Square("e1")) {
                    w_rooks.unset_square(Square("h1"));
                    w_rooks.set_square(Square("f1"));
                } else if (to == Square("c1") && from == Square("e1")) {
                    w_rooks.unset_square(Square("a1"));
                    w_rooks.set_square(Square("d1"));
                }
            } else if (from == b_king) {
                b_king = to;
                castling.unset_black();
                if (to == Square("g8") && from == Square("e8")) {
                    b_rooks.unset_square(Square("h8"));
                    b_rooks.set_square(Square("f8"));
                } else if (to == Square("c8") && from == Square("e8")) {
                    b_rooks.unset_square(Square("a8"));
                    b_rooks.set_square(Square("d8"));
                }
            }

            // clear castling rights
            if (w_turn && from == Square("a1")) {
                castling.unset_white_queenside();
            } else if (w_turn && from == Square("h1")) {
                castling.unset_white_kingside();
            } else if (!w_turn && from == Square("a8")) {
                castling.unset_black_queenside();
            } else if (!w_turn && from == Square("h8")) {
                castling.unset_black_kingside();
            }

            // set en-passant flags
            if (w_turn) {
                w_pawns.set_square_if(Square(0, to.get_col()), w_pawns.get_square(from) && (to.get_row() - from.get_row()) == 2);
            } else {
                b_pawns.set_square_if(Square(7, to.get_col()), b_pawns.get_square(from) && (from.get_row() - to.get_row()) == 2);
            }

            // clear en-passant captures
            if (w_turn && w_pawns.get_square(from) && (to.get_col() != from.get_col()) && b_pawns.get_square(7, to.get_col())) {
                b_pawns.unset_square(Square(4, to.get_col()));
            } else if (!w_turn && b_pawns.get_square(from) && (to.get_col() != from.get_col()) && w_pawns.get_square(0, to.get_col())) {
                w_pawns.unset_square(Square(3, to.get_col()));
            }

            // add to square 
            if (w_turn) {
                w_pawns.set_square_if(to, w_pawns.get_square(from));
                w_rooks.set_square_if(to, w_rooks.get_square(from));
                w_knights.set_square_if(to, w_knights.get_square(from));
                w_bishops.set_square_if(to, w_bishops.get_square(from));
            } else {
                b_pawns.set_square_if(to, b_pawns.get_square(from));
                b_rooks.set_square_if(to, b_rooks.get_square(from));
                b_knights.set_square_if(to, b_knights.get_square(from));
                b_bishops.set_square_if(to, b_bishops.get_square(from));
            }

            // clear from square
            if (w_turn) {
                w_pawns.unset_square(from);
                w_rooks.unset_square(from);
                w_knights.unset_square(from);
                w_bishops.unset_square(from);
            } else {
                b_pawns.unset_square(from);
                b_rooks.unset_square(from);
                b_knights.unset_square(from);
                b_bishops.unset_square(from);
            }

            // clear capture
             if (w_turn && black_pieces().get_square(to)) {
                reset_50 = true;
                b_pawns.unset_square(to);
                b_rooks.unset_square(to);
                b_knights.unset_square(to);
                b_bishops.unset_square(to);
            } else if (!w_turn && white_pieces().get_square(to)) {
                reset_50 = true;
                w_pawns.unset_square(to);
                w_rooks.unset_square(to);
                w_knights.unset_square(to);
                w_bishops.unset_square(to);
            }

            // promote pawns
            Ply::Promotion promotion = ply.promotion();
            if (promotion != Ply::Promotion::None) {
                if (w_turn) {
                    w_pawns.unset_square(to);
                    if (promotion == Ply::Promotion::Queen) {
                        w_bishops.set_square(to);
                        w_rooks.set_square(to);
                    } else if (promotion == Ply::Promotion::Knight) {
                        w_knights.set_square(to);
                    } else if (promotion == Ply::Promotion::Rook) {
                        w_rooks.set_square(to);
                    } else if (promotion == Ply::Promotion::Bishop) {
                        w_bishops.set_square(to);
                    }
                } else {
                    b_pawns.unset_square(to);
                    if (promotion == Ply::Promotion::Queen) {
                        b_bishops.set_square(to);
                        b_rooks.set_square(to);
                    } else if (promotion == Ply::Promotion::Knight) {
                        b_knights.set_square(to);
                    } else if (promotion == Ply::Promotion::Rook) {
                        b_rooks.set_square(to);
                    } else if (promotion == Ply::Promotion::Bishop) {
                        b_bishops.set_square(to);
                    }
                }
            }

            //clears en-passant flags
            if (w_turn) {
                b_pawns = squarewise_and(b_pawns, pawn_mask);
            } else {
                w_pawns = squarewise_and(w_pawns, pawn_mask);
            }

            // reset rule 50
            if (reset_50) { rule_fifty_ply_clock = 0; } else { rule_fifty_ply_clock++; }
            // increment move count
            if (!w_turn) { move_count++; }
            // change turn
            w_turn = !w_turn;
        }

        std::vector<Ply> generate_potential_plies() {
            std::vector<Ply> ply_list;
            ply_list.reserve(50);
            
            Square king;
            Bitboard our_pieces;
            Bitboard our_pawns;
            Bitboard our_knights;
            Bitboard our_rooks;
            Bitboard our_bishops;
            Bitboard opponent_pieces;
            int forward;
            Bitboard all = all_pieces();
            if (w_turn) {
                king = w_king;
                our_pawns = w_pawns;
                our_knights = w_knights;
                our_rooks = w_rooks;
                our_bishops = w_bishops;
                our_pieces = white_pieces();
                opponent_pieces = black_pieces();
                forward = 1;
            } else {
                king = b_king;
                our_pawns = b_pawns;
                our_knights = b_knights;
                our_rooks = b_rooks;
                our_bishops = b_bishops;
                our_pieces = black_pieces();
                opponent_pieces = white_pieces();
                forward = -1;
            }

            // King moves
            int row = king.get_row();
            int col = king.get_col();
            for (int r = row == 0 ? 0 : -1; r <= (row == 7 ? 0 : 1); r++) {
                for (int c = col == 0 ? 0 : -1; c <= (col == 7 ? 0 : 1); c++) {
                    if (r == 0 && c == 0) { continue; }
                    Square to = Square(row + r, c + col);
                    if (our_pieces.get_square(to)) { continue; }
                    ply_list.push_back(Ply(king, to));
                }
            }

            for (int r = 0; r <= 7; r++) {
                for (int c = 0; c <= 7; c++) {
                    // Bishop moves (and Queens)
                    if (our_bishops.get_square(r,c)) {
                        Square from = Square(r, c);
                        for (int diff = 1; (r + diff) <= 7 && (c + diff) <= 7; diff++) {
                            Square to = Square(r + diff, c + diff);
                            if (!our_pieces.get_square(to)) {
                                ply_list.push_back(Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                        for (int diff = 1; (r + diff) <= 7 && (c - diff) >= 0; diff++) {
                            Square to = Square(r + diff, c - diff);
                            if (!our_pieces.get_square(to)) {
                                ply_list.push_back(Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                        for (int diff = 1; (r - diff) >= 0 && (c + diff) <= 7; diff++) {
                            Square to = Square(r - diff, c + diff);
                            if (!our_pieces.get_square(to)) {
                                ply_list.push_back(Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                        for (int diff = 1; (r - diff) >= 0 && (c - diff) >= 0; diff++) {
                            Square to = Square(r - diff, c - diff);
                            if (!our_pieces.get_square(to)) {
                                ply_list.push_back(Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                    } // (no "else" b/c can be bishop and rook on same square)
                    // Pawn moves
                    if (our_pawns.get_square(r, c)) {
                        Square from = Square(r, c);
                        Square to = Square(r + forward, c);
                        if (!all.get_square(to)) {
                            add_pawn_plies(&ply_list, from, to);
                            if ((w_turn && r == 1) || (!w_turn && r == 6)) {
                                to = Square(r + forward * 2, c);
                                if (!all.get_square(to)) {
                                    ply_list.push_back(Ply(from, to));
                                }

                            }
                        }
                        if (c != 7) {
                            to = Square(r + forward, c + 1);
                            if (opponent_pieces.get_square(to)) {
                               add_pawn_plies(&ply_list, from, to);
                            }
                            if (r == (w_turn ? 4 : 3) && all_en_passant().get_square(w_turn ? 7 : 0, c + 1)) {
                                ply_list.push_back(Ply(from, to));
                            }
                        }
                        if (c != 0) {
                            to = Square(r + forward, c - 1);
                            if (opponent_pieces.get_square(to)) {
                                add_pawn_plies(&ply_list, from, to);
                            }
                            if (r == (w_turn ? 4 : 3) && all_en_passant().get_square(w_turn ? 7 : 0, c - 1)) {
                                ply_list.push_back(Ply(from, to));
                            }
                        }
                    }
                    // Knight moves
                    else if (our_knights.get_square(r, c)) {
                        Square from = Square(r, c);
                        for (Square to : knight_attacks[from.get_int_value()]) {
                            if (!our_pieces.get_square(to)) {
                                ply_list.push_back(Ply(from, to));
                            }
                        }
                    }
                    // Rook moves (and Queens)
                    else if (our_rooks.get_square(r,c)) {
                        Square from = Square(r, c);
                        for (int r_target = r + 1; r_target <= 7; r_target++) {
                            Square to = Square(r_target, c);
                            if (!our_pieces.get_square(to)) {
                                ply_list.push_back(Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                        for (int r_target = r - 1; r_target >= 0; r_target--) {
                            Square to = Square(r_target, c);
                            if (!our_pieces.get_square(to)) {
                                ply_list.push_back(Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                        for (int c_target = c + 1; c_target <= 7; c_target++) {
                            Square to = Square(r, c_target);
                            if (!our_pieces.get_square(to)) {
                                ply_list.push_back(Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                        for (int c_target = c - 1; c_target >= 0; c_target--) {
                            Square to = Square(r, c_target);
                            if (!our_pieces.get_square(to)) {
                                ply_list.push_back(Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                    }
                }
            }
            return ply_list;
        }

        std::string to_fen() const {
            std::string result;
            int empty = 0;
            for (int row = 7; row >= 0; row--) {
                for (int col = 0; col < 8; col++) {
                    if (all_pieces().get_square(row, col) == 0) {
                        empty++;
                    } else {
                        if (empty != 0) {
                            result += empty + 48;
                        }
                        empty = 0;
                        result += square_to_char(row, col);
                    }
                }
                if (empty != 0) {
                    result += empty + 48;
                }
                empty = 0;
                if (row != 0) {
                    result += '/';
                }
            }

            result += w_turn ? " w " : " b ";

            if (castling.no_castle_legal()) {
                result += '-';
            } else {
                if (castling.get_white_kingside()) { result += "K"; };
                if (castling.get_white_queenside()) { result += "Q"; };
                if (castling.get_black_kingside()) { result += "k"; };
                if (castling.get_black_queenside()) { result += "q"; };
            }
            result += ' ';

            if (all_en_passant().empty()) {
                result += "- ";
            } else if (!white_en_passant().empty()) {
                for (int col = 0; col < 8; col++) {
                    if (w_pawns.get_square(0, col)) {
                        result += (col + 'a');
                        result += "3 ";
                        break;
                    }
                }
            } else if (!black_en_passant().empty()) {
                for (int col = 0; col < 8; col++) {
                    if (b_pawns.get_square(7, col)) {
                        result += (col + 'a');
                        result += "6 ";
                        break;
                    }
                }
            }
            result += std::to_string(rule_fifty_ply_clock);
            result += ' ';
            result += std::to_string(move_count);
            return result;
        }

        std::string to_string() const {
            std::string result;
            for (int row = 7; row >= 0; row--) {
                for (int col = 0; col < 8; col++) {
                    result += square_to_char(row, col);
                }
                result += '\n';
            }
            return result;
        }
    
    private:
       // ranks 1 and 8 on pawns track en-passant
        const Bitboard pawn_mask = Bitboard(0x00FFFFFFFFFFFF00);
        const Bitboard en_passant_mask = Bitboard(0xFF000000000000FF);

        Bitboard w_pawns;
        Bitboard w_rooks; // includes Queens
        Bitboard w_knights;
        Bitboard w_bishops; // includes Queens
        Square w_king;

        Bitboard b_pawns;
        Bitboard b_rooks; // includes Queens
        Bitboard b_knights;
        Bitboard b_bishops; // includes Queens
        Square b_king;

        bool w_turn;

        Castling castling;

        int rule_fifty_ply_clock = 0;
        int move_count = 1;

        char square_to_char(int row, int col) const {
            if (row == w_king.get_row() && col == w_king.get_col()) {
                return 'K';
            } else if (white_queens().get_square(row, col)) {
                return 'Q';
            } else if (white_pawns().get_square(row, col)) {
                return 'P';
            } else if (w_rooks.get_square(row, col)) {
                return 'R';
            } else if (w_knights.get_square(row, col)) {
                return 'N';
            } else if (w_bishops.get_square(row, col)) {
                return 'B';
            } else if (row == b_king.get_row() && col == b_king.get_col()) {
                return 'k';
            } else if (black_queens().get_square(row, col)) {
                return 'q';
            } else if (black_pawns().get_square(row, col)) {
                return 'p';
            } else if (b_rooks.get_square(row, col)) {
                return 'r';
            } else if (b_knights.get_square(row, col)) {
                return 'n';
            } else if (b_bishops.get_square(row, col)) {
                return 'b';
            } else {
                return '.';
            }
        }

        void add_pawn_plies(std::vector<Ply>* ply_list, Square from, Square to) {
            if (to.get_row() == 7 || to.get_row() == 0) {
                ply_list->push_back(Ply(from, to, Ply::Promotion::Queen));
                ply_list->push_back(Ply(from, to, Ply::Promotion::Knight));
                ply_list->push_back(Ply(from, to, Ply::Promotion::Rook));
                ply_list->push_back(Ply(from, to, Ply::Promotion::Bishop));
            } else {
                ply_list->push_back(Ply(from, to));
            }
        }
};
