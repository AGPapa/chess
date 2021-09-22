#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include "piece_attacks.hpp"
#include "board_signature.cpp"
#include "ply.cpp"

// ranks 1 and 8 on pawns track en-passant
static const Bitboard pawn_mask = Bitboard(0x00FFFFFFFFFFFF00);
static const Bitboard en_passant_mask = Bitboard(0xFF000000000000FF);
static const Bitboard light_squares_mask(0x55AA55AA55AA55AA);
static const Bitboard dark_squares_mask(0xAA55AA55AA55AA55);

static const Square a1 = Square("a1");
static const Square c1 = Square("c1");
static const Square d1 = Square("d1");
static const Square e1 = Square("e1");
static const Square f1 = Square("f1");
static const Square g1 = Square("g1");
static const Square h1 = Square("h1");

static const Square a8 = Square("a8");
static const Square c8 = Square("c8");
static const Square d8 = Square("d8");
static const Square e8 = Square("e8");
static const Square f8 = Square("f8");
static const Square g8 = Square("g8");
static const Square h8 = Square("h8");

class Board {

    public:
        Board() {};

        Board(Bitboard w_p, Bitboard b_p, Bitboard p, Bitboard b, Bitboard r, Bitboard n,
              Square w_k, Square b_k, bool w_t, Castling c, int r50_ply, int m_c) {
            w_pieces = w_p;
            b_pieces = b_p;
            pawns = p;
            bishops = b;
            rooks = r;
            knights = n;

            w_king = w_k;
            b_king = b_k;

            w_turn = w_t;
            castling = c;

            rule_fifty_ply_clock = r50_ply;
            move_count = m_c;
        };
        
        Board(Bitboard w_p, Bitboard w_r, Bitboard w_n, Bitboard w_b, Square w_k,
              Bitboard b_p, Bitboard b_r, Bitboard b_n, Bitboard b_b, Square b_k,
              bool w_t, Castling c, int r50_ply, int m_c) {

            w_pieces = squarewise_or(squarewise_or(squarewise_and(w_p, pawn_mask), w_r), squarewise_or(w_n, w_b));
            w_pieces.set_square(w_k);

            b_pieces = squarewise_or(squarewise_or(squarewise_and(b_p, pawn_mask), b_r), squarewise_or(b_n, b_b));
            b_pieces.set_square(b_k);

            pawns = squarewise_or(w_p, b_p);
            bishops = squarewise_or(w_b, b_b);
            rooks = squarewise_or(w_r, b_r);
            knights = squarewise_or(w_n, b_n);

            w_king = w_k;
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
                    w_pieces.set_square(row, col);
                } else if (c == 'Q') {
                    bishops.set_square(row, col);
                    rooks.set_square(row, col);
                    w_pieces.set_square(row, col);
                } else if (c == 'P') {
                    pawns.set_square(row, col);
                    w_pieces.set_square(row, col);
                } else if (c == 'R') {
                    rooks.set_square(row, col);
                    w_pieces.set_square(row, col);
                } else if (c == 'N') {
                    knights.set_square(row, col);
                    w_pieces.set_square(row, col);
                } else if (c == 'B') {
                    bishops.set_square(row, col);
                    w_pieces.set_square(row, col);
                } else if (c == 'k') {
                    b_king = Square(row, col);
                    b_pieces.set_square(row, col);
                } else if (c == 'q') {
                    bishops.set_square(row, col);
                    rooks.set_square(row, col);
                    b_pieces.set_square(row, col);
                } else if (c == 'p') {
                    pawns.set_square(row, col);
                    b_pieces.set_square(row, col);
                } else if (c == 'r') {
                    rooks.set_square(row, col);
                    b_pieces.set_square(row, col);
                } else if (c == 'n') {
                    knights.set_square(row, col);
                    b_pieces.set_square(row, col);
                } else if (c == 'b') {
                    bishops.set_square(row, col);
                    b_pieces.set_square(row, col);
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
                pawns.set_square(0, 0);
            } else if (en_passant == "b3") {
                pawns.set_square(0, 1);
            } else if (en_passant == "c3") {
                pawns.set_square(0, 2);
            } else if (en_passant == "d3") {
                pawns.set_square(0, 3);
            } else if (en_passant == "e3") {
                pawns.set_square(0, 4);
            } else if (en_passant == "f3") {
                pawns.set_square(0, 5);
            } else if (en_passant == "g3") {
                pawns.set_square(0, 6);
            } else if (en_passant == "h3") {
                pawns.set_square(0, 7);
            } else if (en_passant == "a6") {
                pawns.set_square(7, 0);
            } else if (en_passant == "b6") {
                pawns.set_square(7, 1);
            } else if (en_passant == "c6") {
                pawns.set_square(7, 2);
            } else if (en_passant == "d6") {
                pawns.set_square(7, 3);
            } else if (en_passant == "e6") {
                pawns.set_square(7, 4);
            } else if (en_passant == "f6") {
                pawns.set_square(7, 5);
            } else if (en_passant == "g6") {
                pawns.set_square(7, 6);
            } else if (en_passant == "h6") {
                pawns.set_square(7, 7);
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

        Board copy_without_history() const {
            return Board(w_pieces, b_pieces, pawns, bishops, rooks, knights,
                w_king, b_king, w_turn, castling, rule_fifty_ply_clock, move_count);
        }

        BoardSignature signature() const {
            return BoardSignature(w_pieces, b_pieces, bishops, rooks, knights, w_king, b_king, castling);
        }

        Square white_king() const {
            return w_king;
        }

        Square black_king() const {
            return b_king;
        }

        Bitboard white_pawns() const {
            return squarewise_and(w_pieces, squarewise_and(pawns, pawn_mask));
        }

        Bitboard black_pawns() const {
            return squarewise_and(b_pieces, squarewise_and(pawns, pawn_mask));
        }

        Bitboard all_pawns() const {
            return squarewise_and(pawns, pawn_mask);
        }

        Bitboard white_knights() const {
            return squarewise_and(w_pieces, knights);
        }

        Bitboard black_knights() const {
            return squarewise_and(b_pieces, knights);
        }

        Bitboard white_bishops() const {
            return squarewise_and(rooks.squarewise_not(), squarewise_and(w_pieces, bishops));
        }

        Bitboard black_bishops() const {
            return squarewise_and(rooks.squarewise_not(), squarewise_and(b_pieces, bishops));
        }

        Bitboard white_rooks() const {
            return squarewise_and(bishops.squarewise_not(), squarewise_and(w_pieces, rooks));
        }

        Bitboard black_rooks() const {
            return  squarewise_and(bishops.squarewise_not(), squarewise_and(b_pieces, rooks));
        }

        Bitboard white_queens() const {
            return squarewise_and(w_pieces, squarewise_and(bishops, rooks));
        }

        Bitboard black_queens() const {
            return squarewise_and(b_pieces, squarewise_and(bishops, rooks));
        }

        Bitboard white_en_passant() const {
            return squarewise_and(w_pieces, squarewise_and(pawns, en_passant_mask));
        }

        Bitboard black_en_passant() const {
            return squarewise_and(b_pieces, squarewise_and(pawns, en_passant_mask));
        }

        Bitboard all_en_passant() const {
            return squarewise_and(pawns, en_passant_mask);
        }

        Bitboard all_pieces() const {
            Bitboard b = squarewise_or(w_pieces, b_pieces);
            b.set_square(w_king);
            b.set_square(b_king);
            return b;
        }

        Bitboard white_pieces() const {
            return w_pieces;
        }

        Bitboard black_pieces() const {
            return b_pieces;
        }

        bool can_white_castle_kingside() const {
            return castling.get_white_kingside();
        }

        bool can_white_castle_queenside() const {
            return castling.get_white_queenside();
        }

        bool can_black_castle_kingside() const {
            return castling.get_black_kingside();
        }

        bool can_black_castle_queenside() const {
            return castling.get_black_queenside();
        }

        bool is_white_turn() const {
            return w_turn;
        }

        Board mirror() {
            return Board(b_pieces.mirror(), w_pieces.mirror(), pawns.mirror(), bishops.mirror(), rooks.mirror(), knights.mirror(),
                            b_king.mirror(), w_king.mirror(), !w_turn, castling.mirror(), rule_fifty_ply_clock, move_count);
        }

        void apply_ply(Ply ply) {
            board_history.push_front(signature());
            apply_ply_without_history(ply);
        }

        void apply_ply_without_history(Ply ply) {
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
                w_pieces.unset_square(from);
                w_pieces.set_square(to);
                w_king = to;
                castling.unset_white();
                if (to == g1 && from == e1) {
                    rooks.unset_square(h1);
                    w_pieces.unset_square(h1);
                    rooks.set_square(f1);
                    w_pieces.set_square(f1);
                } else if (to == c1 && from == e1) {
                    rooks.unset_square(a1);
                    w_pieces.unset_square(a1);
                    rooks.set_square(d1);
                    w_pieces.set_square(d1);
                }
            } else if (from == b_king) {
                b_king = to;
                castling.unset_black();
                if (to == g8 && from == e8) {
                    rooks.unset_square(h8);
                    b_pieces.unset_square(h8);
                    rooks.set_square(f8);
                    b_pieces.set_square(f8);
                } else if (to == c8 && from == e8) {
                    rooks.unset_square(a8);
                    b_pieces.unset_square(a8);
                    rooks.set_square(d8);
                    b_pieces.set_square(d8);
                }
            }

            // clear castling rights
            if (from == a1) {
                castling.unset_white_queenside();
            } else if (from == h1) {
                castling.unset_white_kingside();
            } else if (from == a8) {
                castling.unset_black_queenside();
            } else if (from == h8) {
                castling.unset_black_kingside();
            }
            if (to == a1) {
                castling.unset_white_queenside();
            } else if (to == h1) {
                castling.unset_white_kingside();
            } else if (to == a8) {
                castling.unset_black_queenside();
            } else if (to == h8) {
                castling.unset_black_kingside();
            }

            // clear en-passant captures
            if (w_turn && pawns.get_square(from) && (to.get_col() != from.get_col()) && pawns.get_square(7, to.get_col())) {
                pawns.unset_square(Square(4, to.get_col()));
                b_pieces.unset_square(Square(4, to.get_col()));
            } else if (!w_turn && pawns.get_square(from) && (to.get_col() != from.get_col()) && pawns.get_square(0, to.get_col())) {
                pawns.unset_square(Square(3, to.get_col()));
                w_pieces.unset_square(Square(3, to.get_col()));
            }

            //clears en-passant flags
            pawns = squarewise_and(pawns, pawn_mask);

            // set en-passant flags
            if (w_turn) {
                pawns.set_square_if(Square(0, to.get_col()), pawns.get_square(from) && (to.get_row() - from.get_row()) == 2);
            } else {
                pawns.set_square_if(Square(7, to.get_col()), pawns.get_square(from) && (from.get_row() - to.get_row()) == 2);
            }

            // clear capture
            if (w_turn && black_pieces().get_square(to)) {
                reset_50 = true;
                pawns.unset_square(to);
                rooks.unset_square(to);
                knights.unset_square(to);
                bishops.unset_square(to);
                b_pieces.unset_square(to);
            } else if (!w_turn && white_pieces().get_square(to)) {
                reset_50 = true;
                pawns.unset_square(to);
                rooks.unset_square(to);
                knights.unset_square(to);
                bishops.unset_square(to);
                w_pieces.unset_square(to);
            }

            // add to square
            pawns.set_square_if(to, pawns.get_square(from));
            rooks.set_square_if(to, rooks.get_square(from));
            knights.set_square_if(to, knights.get_square(from));
            bishops.set_square_if(to, bishops.get_square(from));
            w_pieces.set_square_if(to, w_turn);
            b_pieces.set_square_if(to, !w_turn);

            // clear from square
            pawns.unset_square(from);
            rooks.unset_square(from);
            knights.unset_square(from);
            bishops.unset_square(from);
            w_pieces.unset_square(from);
            b_pieces.unset_square(from);

            // promote pawns
            Ply::Promotion promotion = ply.promotion();
            if (promotion != Ply::Promotion::None) {
                pawns.unset_square(to);
                if (promotion == Ply::Promotion::Queen) {
                    bishops.set_square(to);
                    rooks.set_square(to);
                } else if (promotion == Ply::Promotion::Knight) {
                    knights.set_square(to);
                } else if (promotion == Ply::Promotion::Rook) {
                    rooks.set_square(to);
                } else if (promotion == Ply::Promotion::Bishop) {
                    bishops.set_square(to);
                }
            }

            // reset rule 50
            if (reset_50) { rule_fifty_ply_clock = 0; board_history.clear(); } else { rule_fifty_ply_clock++; }
            // increment move count
            if (!w_turn) { move_count++; }
            // change turn
            w_turn = !w_turn;
        }

        bool is_white_king_in_check() const {
            return square_under_attack(w_king, !w_turn);
        }

        bool is_black_king_in_check() const {
            return square_under_attack(b_king, !w_turn);
        }

        bool square_under_attack(Square s) const {
            return square_under_attack(s, w_turn);
        }

        bool square_under_attack(Square s, bool white_turn) const {
            int r = s.get_row();
            int c = s.get_col();

            Bitboard our_knights;
            Bitboard our_rooks;
            Bitboard our_bishops;
            Bitboard our_pawns;
            Square our_king;
            int our_forward;
            Bitboard all = squarewise_or(w_pieces, b_pieces);
            if (white_turn) {
                our_forward = 1;
                our_knights = squarewise_and(w_pieces, knights);
                our_rooks = squarewise_and(w_pieces, rooks);
                our_bishops = squarewise_and(w_pieces, bishops);
                our_pawns = squarewise_and(w_pieces, pawns);
                our_king = w_king;
            } else {
                our_forward = -1;
                our_knights = squarewise_and(b_pieces, knights);
                our_rooks = squarewise_and(b_pieces, rooks);
                our_bishops = squarewise_and(b_pieces, bishops);
                our_pawns = squarewise_and(b_pieces, pawns);
                our_king = b_king;
            }

            // Knights
            if (!our_knights.empty()) {
                for (Square to : knight_attacks[s.get_int_value()]) {
                    if (our_knights.get_square(to)) {
                        return true;
                    }
                }
            }

            // Rooks (and Queens)
            if (!our_rooks.empty()) {
                for (int r_target = r + 1; r_target <= 7; r_target++) {
                    Square to = Square(r_target, c);
                    if (our_rooks.get_square(to)) {
                        return true;
                    }
                    if (all.get_square(to)) {
                        break;
                    }
                }
                for (int r_target = r - 1; r_target >= 0; r_target--) {
                    Square to = Square(r_target, c);
                    if (our_rooks.get_square(to)) {
                        return true;
                    }
                    if (all.get_square(to)) {
                        break;
                    }
                }
                for (int c_target = c + 1; c_target <= 7; c_target++) {
                    Square to = Square(r, c_target);
                    if (our_rooks.get_square(to)) {
                        return true;
                    }
                    if (all.get_square(to)) {
                        break;
                    }
                }
                for (int c_target = c - 1; c_target >= 0; c_target--) {
                    Square to = Square(r, c_target);
                    if (our_rooks.get_square(to)) {
                        return true;
                    }
                    if (all.get_square(to)) {
                        break;
                    }
                }
            }

            // Bishops (and Queens)
            if (!our_bishops.empty()) {
                Square from = Square(r, c);
                for (int diff = 1; (r + diff) <= 7 && (c + diff) <= 7; diff++) {
                    Square to = Square(r + diff, c + diff);
                    if (our_bishops.get_square(to)) {
                        return true;
                    }
                    if (all.get_square(to)) {
                        break;
                    }
                }
                for (int diff = 1; (r + diff) <= 7 && (c - diff) >= 0; diff++) {
                    Square to = Square(r + diff, c - diff);
                    if (our_bishops.get_square(to)) {
                        return true;
                    }
                    if (all.get_square(to)) {
                        break;
                    }
                }
                for (int diff = 1; (r - diff) >= 0 && (c + diff) <= 7; diff++) {
                    Square to = Square(r - diff, c + diff);
                    if (our_bishops.get_square(to)) {
                        return true;
                    }
                    if (all.get_square(to)) {
                        break;
                    }
                }
                for (int diff = 1; (r - diff) >= 0 && (c - diff) >= 0; diff++) {
                    Square to = Square(r - diff, c - diff);
                    if (our_bishops.get_square(to)) {
                        return true;
                    }
                    if (all.get_square(to)) {
                        break;
                    }
                }
            }

            // Pawns - does not consider en-passant!! (doesn't need to for our uses)
            if (c != 7 && our_pawns.get_square(r - our_forward, c + 1)) {
                return true;
            }
            if (c != 0 && our_pawns.get_square(r - our_forward, c - 1)) {
                return true;
            }

            // King - does not consider if king would be in check!! (doesn't need to for our uses)
            if (std::abs(r - our_king.get_row()) <= 1 && std::abs(c - our_king.get_col()) <= 1 ) {
                return true;
            }
            return false;
        }

        std::vector<Ply> generate_potential_plies() const {
            std::vector<Ply> ply_list;
            ply_list.reserve(50);
            
            Square king;
            Bitboard our_pieces;
            Bitboard our_pawns;
            Bitboard our_knights;
            Bitboard our_rooks;
            Bitboard our_bishops;
            Bitboard opponent_pieces;
            Bitboard all = all_pieces();
            int forward;
            bool can_castle_kingside;
            bool can_castle_queenside;
            int home_row;
            if (w_turn) {
                king = w_king;
                our_pawns = white_pawns();
                our_knights = squarewise_and(w_pieces, knights);
                our_rooks = squarewise_and(w_pieces, rooks);
                our_bishops = squarewise_and(w_pieces, bishops);
                our_pieces = white_pieces();
                opponent_pieces = black_pieces();
                forward = 1;
                can_castle_kingside = castling.get_white_kingside();
                can_castle_queenside = castling.get_white_queenside();
                home_row = 0;
            } else {
                king = b_king;
                our_pawns = black_pawns();
                our_knights = squarewise_and(b_pieces, knights);
                our_rooks = squarewise_and(b_pieces, rooks);
                our_bishops = squarewise_and(b_pieces, bishops);
                our_pieces = black_pieces();
                opponent_pieces = white_pieces();
                forward = -1;
                can_castle_kingside = castling.get_black_kingside();
                can_castle_queenside = castling.get_black_queenside();
                home_row = 7;
            }

            // King moves
            int row = king.get_row();
            int col = king.get_col();
            for (int r = row == 0 ? 0 : -1; r <= (row == 7 ? 0 : 1); r++) {
                for (int c = col == 0 ? 0 : -1; c <= (col == 7 ? 0 : 1); c++) {
                    if (r == 0 && c == 0) { continue; }
                    Square to = Square(row + r, c + col);
                    if (our_pieces.get_square(to)) { continue; }
                    add_ply_if_king_not_in_check(&ply_list, Ply(king, to));
                }
            }
            // Castling
            if (can_castle_kingside) {
                if (!all.get_square(home_row, 5) && !all.get_square(home_row, 6)
                    && !square_under_attack(king, !w_turn)
                    && !square_under_attack(Square(home_row, 5), !w_turn)
                    && !square_under_attack(Square(home_row, 6), !w_turn)
                ) {
                    ply_list.push_back(Ply(king, Square(home_row, 6)));
                }
            }
            if (can_castle_queenside) {
                if (!all.get_square(home_row, 1) && !all.get_square(home_row, 2) && !all.get_square(home_row, 3)
                    && !square_under_attack(king, !w_turn)
                    && !square_under_attack(Square(home_row, 2), !w_turn)
                    && !square_under_attack(Square(home_row, 3), !w_turn)
                ) {
                    ply_list.push_back(Ply(king, Square(home_row, 2)));
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
                                add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                        for (int diff = 1; (r + diff) <= 7 && (c - diff) >= 0; diff++) {
                            Square to = Square(r + diff, c - diff);
                            if (!our_pieces.get_square(to)) {
                                add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                        for (int diff = 1; (r - diff) >= 0 && (c + diff) <= 7; diff++) {
                            Square to = Square(r - diff, c + diff);
                            if (!our_pieces.get_square(to)) {
                                add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                        for (int diff = 1; (r - diff) >= 0 && (c - diff) >= 0; diff++) {
                            Square to = Square(r - diff, c - diff);
                            if (!our_pieces.get_square(to)) {
                                add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
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
                                    add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
                                }

                            }
                        }
                        if (c != 7) {
                            to = Square(r + forward, c + 1);
                            if (opponent_pieces.get_square(to)) {
                               add_pawn_plies(&ply_list, from, to);
                            }
                            if (r == (w_turn ? 4 : 3) && all_en_passant().get_square(w_turn ? 7 : 0, c + 1)) {
                                add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
                            }
                        }
                        if (c != 0) {
                            to = Square(r + forward, c - 1);
                            if (opponent_pieces.get_square(to)) {
                                add_pawn_plies(&ply_list, from, to);
                            }
                            if (r == (w_turn ? 4 : 3) && all_en_passant().get_square(w_turn ? 7 : 0, c - 1)) {
                                add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
                            }
                        }
                    }
                    // Knight moves
                    else if (our_knights.get_square(r, c)) {
                        Square from = Square(r, c);
                        for (Square to : knight_attacks[from.get_int_value()]) {
                            if (!our_pieces.get_square(to)) {
                                add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
                            }
                        }
                    }
                    // Rook moves (and Queens)
                    else if (our_rooks.get_square(r,c)) {
                        Square from = Square(r, c);
                        for (int r_target = r + 1; r_target <= 7; r_target++) {
                            Square to = Square(r_target, c);
                            if (!our_pieces.get_square(to)) {
                                add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                        for (int r_target = r - 1; r_target >= 0; r_target--) {
                            Square to = Square(r_target, c);
                            if (!our_pieces.get_square(to)) {
                                add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                        for (int c_target = c + 1; c_target <= 7; c_target++) {
                            Square to = Square(r, c_target);
                            if (!our_pieces.get_square(to)) {
                                add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
                            }
                            if (all.get_square(to)) {
                                break;
                            }
                        }
                        for (int c_target = c - 1; c_target >= 0; c_target--) {
                            Square to = Square(r, c_target);
                            if (!our_pieces.get_square(to)) {
                                add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
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
                    if (pawns.get_square(0, col)) {
                        result += (col + 'a');
                        result += "3 ";
                        break;
                    }
                }
            } else if (!black_en_passant().empty()) {
                for (int col = 0; col < 8; col++) {
                    if (pawns.get_square(7, col)) {
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

        bool is_threefold_repetition() const {
            if (board_history.size() < 8) return false;
            BoardSignature current_board = signature();
            int repetition_count = 1;
            // jumps two board signatures at a time to skip white and black turns
            for (std::list<BoardSignature>::const_iterator b = ++board_history.begin(); b != board_history.end(); ++b, ++b) {
                if (current_board.is_repetition(*b)) {
                    if (repetition_count == 2) return true;
                    ++b; ++b; // skips two board signatures after we find a repetition
                    repetition_count++;
                }
            }
            return false;
        }

        bool is_fifty_move_draw() const {
            return rule_fifty_ply_clock >= 100;
        }

        bool is_insufficient_mating_material() const {
            if (!rooks.empty() || !pawns.empty()) {
                return false;
            }

            // three pieces or less, no queens/rooks/pawns
            if ((squarewise_or(w_pieces, b_pieces)).count() < 3) {
                return true;
            }

            // multiple knights
            if (!(knights.empty())) {
                return false;
            }

            // both a light and dark square bishop
            const bool light_bishop = !squarewise_and(bishops, light_squares_mask).empty();
            const bool dark_bishop = !squarewise_and(bishops, dark_squares_mask).empty();
            return !(light_bishop && dark_bishop);
        }
    
    private:
        Bitboard w_pieces;
        Bitboard b_pieces;

        Bitboard pawns;
        Bitboard rooks; // includes Queens
        Bitboard knights;
        Bitboard bishops; // includes Queens
        Square w_king;
        Square b_king;

        bool w_turn;

        Castling castling;

        int rule_fifty_ply_clock = 0;
        int move_count = 1;

        std::list<BoardSignature> board_history;

        char square_to_char(int row, int col) const {
            if (row == w_king.get_row() && col == w_king.get_col()) {
                return 'K';
            } else if (white_queens().get_square(row, col)) {
                return 'Q';
            } else if (white_pawns().get_square(row, col)) {
                return 'P';
            } else if (squarewise_and(w_pieces, rooks).get_square(row, col)) {
                return 'R';
            } else if (squarewise_and(w_pieces, knights).get_square(row, col)) {
                return 'N';
            } else if (squarewise_and(w_pieces, bishops).get_square(row, col)) {
                return 'B';
            } else if (row == b_king.get_row() && col == b_king.get_col()) {
                return 'k';
            } else if (black_queens().get_square(row, col)) {
                return 'q';
            } else if (black_pawns().get_square(row, col)) {
                return 'p';
            } else if (squarewise_and(b_pieces, rooks).get_square(row, col)) {
                return 'r';
            } else if (squarewise_and(b_pieces, knights).get_square(row, col)) {
                return 'n';
            } else if (squarewise_and(b_pieces, bishops).get_square(row, col)) {
                return 'b';
            } else {
                return '.';
            }
        }

        void add_ply_if_king_not_in_check(std::vector<Ply>* ply_list, const Ply ply) const {
            Board b = copy_without_history();
            b.apply_ply_without_history(ply);
            Square king = w_turn ? b.w_king : b.b_king;
            if (!b.square_under_attack(king)) {
                ply_list->push_back(ply);
            }
        }

        // TODO: potential speed improvement since we don't need add_ply_if_king_not_in_check for every one
        void add_pawn_plies(std::vector<Ply>* ply_list, Square from, Square to) const {
            if (to.get_row() == 7 || to.get_row() == 0) {
                add_ply_if_king_not_in_check(ply_list, Ply(from, to, Ply::Promotion::Queen));
                add_ply_if_king_not_in_check(ply_list, Ply(from, to, Ply::Promotion::Knight));
                add_ply_if_king_not_in_check(ply_list, Ply(from, to, Ply::Promotion::Rook));
                add_ply_if_king_not_in_check(ply_list, Ply(from, to, Ply::Promotion::Bishop));
            } else {
                add_ply_if_king_not_in_check(ply_list, Ply(from, to));
            }
        }
};
