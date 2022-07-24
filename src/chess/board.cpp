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
static const Bitboard PAWN_MASK = Bitboard(0x00FFFFFFFFFFFF00);
static const Bitboard EN_PASSANT_MASK = Bitboard(0xFF000000000000FF);
static const Bitboard LIGHT_SQUARES_MASK(0x55AA55AA55AA55AA);
static const Bitboard DARK_SQUARES_MASK(0xAA55AA55AA55AA55);

static const Square A1 = Square("a1");
static const Square C1 = Square("c1");
static const Square D1 = Square("d1");
static const Square E1 = Square("e1");
static const Square F1 = Square("f1");
static const Square G1 = Square("g1");
static const Square H1 = Square("h1");

static const Square A8 = Square("a8");
static const Square C8 = Square("c8");
static const Square D8 = Square("d8");
static const Square E8 = Square("e8");
static const Square F8 = Square("f8");
static const Square G8 = Square("g8");
static const Square H8 = Square("h8");

class Board {

    public:
        Board() {};

        Board(Bitboard w_p, Bitboard b_p, Bitboard p, Bitboard b, Bitboard r, Bitboard n,
              Square w_k, Square b_k, bool w_t, Castling c, int r50_ply, int m_c) {
            _w_pieces = w_p;
            _b_pieces = b_p;
            _pawns = p;
            _bishops = b;
            _rooks = r;
            _knights = n;

            _w_king = w_k;
            _b_king = b_k;

            _w_turn = w_t;
            _castling = c;

            _rule_fifty_ply_clock = r50_ply;
            _move_count = m_c;
        };
        
        Board(Bitboard w_p, Bitboard w_r, Bitboard w_n, Bitboard w_b, Square w_k,
              Bitboard b_p, Bitboard b_r, Bitboard b_n, Bitboard b_b, Square b_k,
              bool w_t, Castling c, int r50_ply, int m_c) {

            _w_pieces = squarewise_or(squarewise_or(squarewise_and(w_p, PAWN_MASK), w_r), squarewise_or(w_n, w_b));
            _w_pieces.set_square(w_k);

            _b_pieces = squarewise_or(squarewise_or(squarewise_and(b_p, PAWN_MASK), b_r), squarewise_or(b_n, b_b));
            _b_pieces.set_square(b_k);

            _pawns = squarewise_or(w_p, b_p);
            _bishops = squarewise_or(w_b, b_b);
            _rooks = squarewise_or(w_r, b_r);
            _knights = squarewise_or(w_n, b_n);

            _w_king = w_k;
            _b_king = b_k;

            _w_turn = w_t;
            _castling = c;

            _rule_fifty_ply_clock = r50_ply;
            _move_count = m_c;
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
                    _w_king = Square(row, col);
                    _w_pieces.set_square(row, col);
                } else if (c == 'Q') {
                    _bishops.set_square(row, col);
                    _rooks.set_square(row, col);
                    _w_pieces.set_square(row, col);
                } else if (c == 'P') {
                    _pawns.set_square(row, col);
                    _w_pieces.set_square(row, col);
                } else if (c == 'R') {
                    _rooks.set_square(row, col);
                    _w_pieces.set_square(row, col);
                } else if (c == 'N') {
                    _knights.set_square(row, col);
                    _w_pieces.set_square(row, col);
                } else if (c == 'B') {
                    _bishops.set_square(row, col);
                    _w_pieces.set_square(row, col);
                } else if (c == 'k') {
                    _b_king = Square(row, col);
                    _b_pieces.set_square(row, col);
                } else if (c == 'q') {
                    _bishops.set_square(row, col);
                    _rooks.set_square(row, col);
                    _b_pieces.set_square(row, col);
                } else if (c == 'p') {
                    _pawns.set_square(row, col);
                    _b_pieces.set_square(row, col);
                } else if (c == 'r') {
                    _rooks.set_square(row, col);
                    _b_pieces.set_square(row, col);
                } else if (c == 'n') {
                    _knights.set_square(row, col);
                    _b_pieces.set_square(row, col);
                } else if (c == 'b') {
                    _bishops.set_square(row, col);
                    _b_pieces.set_square(row, col);
                } else {
                    throw std::runtime_error("Bad fen string - invalid piece char: " + fen);
                }
                col++;
            }

            fen_stream >> turn;
            if (turn == "w") { 
                _w_turn = true;
            } else if (turn == "b") {
                _w_turn = false;
            } else {
                throw std::runtime_error("Bad fen string - invalid turn: " + fen);
            }

            fen_stream >> castlings;
            if (castlings == "-") {
                _castling = Castling(false, false, false, false);
            } else if (castlings == "K") {
                _castling = Castling(true, false, false, false);
            } else if (castlings == "Q") {
                _castling = Castling(false, true, false, false);
            } else if (castlings == "KQ") {
                _castling = Castling(true, true, false, false);
            } else if (castlings == "k") {
                _castling = Castling(false, false, true, false);
            } else if (castlings == "Kk") {
                _castling = Castling(true, false, true, false);
            } else if (castlings == "Qk") {
                _castling = Castling(false, true, true, false);
            } else if (castlings == "KQk") {
                _castling = Castling(true, true, true, false);
            } else if (castlings == "q") {
                _castling = Castling(false, false, false, true);
            } else if (castlings == "Kq") {
                _castling = Castling(true, false, false, true);
            } else if (castlings == "Qq") {
                _castling = Castling(false, true, false, true);
            } else if (castlings == "KQq") {
                _castling = Castling(true, true, false, true);
            } else if (castlings == "kq") {
                _castling = Castling(false, false, true, true);
            } else if (castlings == "Kkq") {
                _castling = Castling(true, false, true, true);
            } else if (castlings == "Qkq") {
                _castling = Castling(false, true, true, true);
            } else if (castlings == "KQkq") {
                _castling = Castling(true, true, true, true);
            } else {
                throw std::runtime_error("Bad fen string - invalid _castling: " + fen);
            }
           
            fen_stream >> en_passant;
            if (en_passant == "a3") {
                _pawns.set_square(0, 0);
            } else if (en_passant == "b3") {
                _pawns.set_square(0, 1);
            } else if (en_passant == "c3") {
                _pawns.set_square(0, 2);
            } else if (en_passant == "d3") {
                _pawns.set_square(0, 3);
            } else if (en_passant == "e3") {
                _pawns.set_square(0, 4);
            } else if (en_passant == "f3") {
                _pawns.set_square(0, 5);
            } else if (en_passant == "g3") {
                _pawns.set_square(0, 6);
            } else if (en_passant == "h3") {
                _pawns.set_square(0, 7);
            } else if (en_passant == "a6") {
                _pawns.set_square(7, 0);
            } else if (en_passant == "b6") {
                _pawns.set_square(7, 1);
            } else if (en_passant == "c6") {
                _pawns.set_square(7, 2);
            } else if (en_passant == "d6") {
                _pawns.set_square(7, 3);
            } else if (en_passant == "e6") {
                _pawns.set_square(7, 4);
            } else if (en_passant == "f6") {
                _pawns.set_square(7, 5);
            } else if (en_passant == "g6") {
                _pawns.set_square(7, 6);
            } else if (en_passant == "h6") {
                _pawns.set_square(7, 7);
            } else if (en_passant != "-") {
                throw std::runtime_error("Bad fen string - invalid en_passant: " + fen);
            }

            fen_stream >> _rule_fifty_ply_clock;
            fen_stream >> _move_count;
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
            return Board(_w_pieces, _b_pieces, _pawns, _bishops, _rooks, _knights,
                _w_king, _b_king, _w_turn, _castling, _rule_fifty_ply_clock, _move_count);
        }

        BoardSignature signature() const {
            return BoardSignature(_w_pieces, _b_pieces, _bishops, _rooks, _knights, _w_king, _b_king, _castling);
        }

        Square white_king() const {
            return _w_king;
        }

        Square black_king() const {
            return _b_king;
        }

        Bitboard white_pawns() const {
            return squarewise_and(_w_pieces, squarewise_and(_pawns, PAWN_MASK));
        }

        Bitboard black_pawns() const {
            return squarewise_and(_b_pieces, squarewise_and(_pawns, PAWN_MASK));
        }

        Bitboard all_pawns() const {
            return squarewise_and(_pawns, PAWN_MASK);
        }

        Bitboard white_knights() const {
            return squarewise_and(_w_pieces, _knights);
        }

        Bitboard black_knights() const {
            return squarewise_and(_b_pieces, _knights);
        }

        Bitboard white_bishops() const {
            return squarewise_and(_rooks.squarewise_not(), squarewise_and(_w_pieces, _bishops));
        }

        Bitboard black_bishops() const {
            return squarewise_and(_rooks.squarewise_not(), squarewise_and(_b_pieces, _bishops));
        }

        Bitboard white_rooks() const {
            return squarewise_and(_bishops.squarewise_not(), squarewise_and(_w_pieces, _rooks));
        }

        Bitboard black_rooks() const {
            return  squarewise_and(_bishops.squarewise_not(), squarewise_and(_b_pieces, _rooks));
        }

        Bitboard white_queens() const {
            return squarewise_and(_w_pieces, squarewise_and(_bishops, _rooks));
        }

        Bitboard black_queens() const {
            return squarewise_and(_b_pieces, squarewise_and(_bishops, _rooks));
        }

        Bitboard white_en_passant() const {
            return squarewise_and(_w_pieces, squarewise_and(_pawns, EN_PASSANT_MASK));
        }

        Bitboard black_en_passant() const {
            return squarewise_and(_b_pieces, squarewise_and(_pawns, EN_PASSANT_MASK));
        }

        Bitboard all_en_passant() const {
            return squarewise_and(_pawns, EN_PASSANT_MASK);
        }

        Bitboard all_pieces() const {
            Bitboard b = squarewise_or(_w_pieces, _b_pieces);
            return b;
        }

        Bitboard white_pieces() const {
            return _w_pieces;
        }

        Bitboard black_pieces() const {
            return _b_pieces;
        }

        bool can_white_castle_kingside() const {
            return _castling.get_white_kingside();
        }

        bool can_white_castle_queenside() const {
            return _castling.get_white_queenside();
        }

        bool can_black_castle_kingside() const {
            return _castling.get_black_kingside();
        }

        bool can_black_castle_queenside() const {
            return _castling.get_black_queenside();
        }

        bool is_white_turn() const {
            return _w_turn;
        }

        Board mirror() const {
            return Board(_b_pieces.mirror(), _w_pieces.mirror(), _pawns.mirror(), _bishops.mirror(), _rooks.mirror(), _knights.mirror(),
                            _b_king.mirror(), _w_king.mirror(), !_w_turn, _castling.mirror(), _rule_fifty_ply_clock, _move_count);
        }

        void apply_ply(Ply ply) {
            board_history.push_front(signature());
            apply_ply_without_history(ply);
        }

        void apply_ply_without_history(Ply ply) {
            Square from = ply.from_square();
            Square to = ply.to_square();

            bool reset_50 = false;

            // clear capture
            if (all_pieces().get_square(to)) {
                reset_50 = true;
                _pawns.unset_square(to);
                _rooks.unset_square(to);
                _knights.unset_square(to);
                _bishops.unset_square(to);
                _b_pieces.unset_square(to);
                _w_pieces.unset_square(to);
            }

            // king move
            if (from == _w_king) {
                _w_pieces.unset_square(from);
                _w_pieces.set_square(to);
                _w_king = to;
                _castling.unset_white();
                if (to == G1 && from == E1) {
                    _rooks.unset_square(H1);
                    _w_pieces.unset_square(H1);
                    _rooks.set_square(F1);
                    _w_pieces.set_square(F1);
                } else if (to == C1 && from == E1) {
                    _rooks.unset_square(A1);
                    _w_pieces.unset_square(A1);
                    _rooks.set_square(D1);
                    _w_pieces.set_square(D1);
                }
            } else if (from == _b_king) {
                _b_pieces.unset_square(from);
                _b_pieces.set_square(to);
                _b_king = to;
                _castling.unset_black();
                if (to == G8 && from == E8) {
                    _rooks.unset_square(H8);
                    _b_pieces.unset_square(H8);
                    _rooks.set_square(F8);
                    _b_pieces.set_square(F8);
                } else if (to == C8 && from == E8) {
                    _rooks.unset_square(A8);
                    _b_pieces.unset_square(A8);
                    _rooks.set_square(D8);
                    _b_pieces.set_square(D8);
                }

                //clears en-passant flags
                _pawns = squarewise_and(_pawns, PAWN_MASK);
            // pawn move
            } else if (_pawns.get_square(from)) {
                // resets 50 move counter
                reset_50 = true;

                // clear en-passant captures
                if (_w_turn && (to.get_col() != from.get_col()) && _pawns.get_square(7, to.get_col())) {
                    _pawns.unset_square(Square(4, to.get_col()));
                    _b_pieces.unset_square(Square(4, to.get_col()));
                } else if (!_w_turn && (to.get_col() != from.get_col()) && _pawns.get_square(0, to.get_col())) {
                    _pawns.unset_square(Square(3, to.get_col()));
                    _w_pieces.unset_square(Square(3, to.get_col()));
                }

                //clears en-passant flags
                _pawns = squarewise_and(_pawns, PAWN_MASK);

                // set en-passant flags
                if (_w_turn) {
                    _pawns.set_square_if(Square(0, to.get_col()), (to.get_row() - from.get_row()) == 2);
                } else {
                    _pawns.set_square_if(Square(7, to.get_col()), (from.get_row() - to.get_row()) == 2);
                }

                // add to square
                _pawns.set_square(to);
                _w_pieces.set_square_if(to, _w_turn);
                _b_pieces.set_square_if(to, !_w_turn);

                // promote _pawns
                Ply::Promotion promotion = ply.promotion();
                if (promotion != Ply::Promotion::None) {
                    _pawns.unset_square(to);
                    if (promotion == Ply::Promotion::Queen) {
                        _bishops.set_square(to);
                        _rooks.set_square(to);
                    } else if (promotion == Ply::Promotion::Knight) {
                        _knights.set_square(to);
                    } else if (promotion == Ply::Promotion::Rook) {
                        _rooks.set_square(to);
                    } else if (promotion == Ply::Promotion::Bishop) {
                        _bishops.set_square(to);
                    }
                }

                // clear from square
                _pawns.unset_square(from);
                _w_pieces.unset_square(from);
                _b_pieces.unset_square(from);
            } else {
                // clear _castling rights
                if (from == A1) {
                    _castling.unset_white_queenside();
                } else if (from == H1) {
                    _castling.unset_white_kingside();
                } else if (from == A8) {
                    _castling.unset_black_queenside();
                } else if (from == H8) {
                    _castling.unset_black_kingside();
                }
                if (to == A1) {
                    _castling.unset_white_queenside();
                } else if (to == H1) {
                    _castling.unset_white_kingside();
                } else if (to == A8) {
                    _castling.unset_black_queenside();
                } else if (to == H8) {
                    _castling.unset_black_kingside();
                }

                //clears en-passant flags
                _pawns = squarewise_and(_pawns, PAWN_MASK);

                // add to square
                _rooks.set_square_if(to, _rooks.get_square(from));
                _knights.set_square_if(to, _knights.get_square(from));
                _bishops.set_square_if(to, _bishops.get_square(from));
                _w_pieces.set_square_if(to, _w_turn);
                _b_pieces.set_square_if(to, !_w_turn);

                // clear from square
                _rooks.unset_square(from);
                _knights.unset_square(from);
                _bishops.unset_square(from);
                _w_pieces.unset_square(from);
                _b_pieces.unset_square(from);
            }

            // reset rule 50
            if (reset_50) { _rule_fifty_ply_clock = 0; board_history.clear(); } else { _rule_fifty_ply_clock++; }
            // increment move count
            if (!_w_turn) { _move_count++; }
            // change turn
            _w_turn = !_w_turn;
        }

        bool is_white_king_in_check() const {
            return square_under_attack(_w_king, !_w_turn);
        }

        bool is_black_king_in_check() const {
            return square_under_attack(_b_king, !_w_turn);
        }

        bool square_under_attack(Square s) const {
            return square_under_attack(s, _w_turn);
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
            Bitboard all = squarewise_or(_w_pieces, _b_pieces);
            if (white_turn) {
                our_forward = 1;
                our_knights = squarewise_and(_w_pieces, _knights);
                our_rooks = squarewise_and(_w_pieces, _rooks);
                our_bishops = squarewise_and(_w_pieces, _bishops);
                our_pawns = squarewise_and(_w_pieces, _pawns);
                our_king = _w_king;
            } else {
                our_forward = -1;
                our_knights = squarewise_and(_b_pieces, _knights);
                our_rooks = squarewise_and(_b_pieces, _rooks);
                our_bishops = squarewise_and(_b_pieces, _bishops);
                our_pawns = squarewise_and(_b_pieces, _pawns);
                our_king = _b_king;
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
            if (_w_turn) {
                king = _w_king;
                our_pawns = white_pawns();
                our_knights = squarewise_and(_w_pieces, _knights);
                our_rooks = squarewise_and(_w_pieces, _rooks);
                our_bishops = squarewise_and(_w_pieces, _bishops);
                our_pieces = white_pieces();
                opponent_pieces = black_pieces();
                forward = 1;
                can_castle_kingside = _castling.get_white_kingside();
                can_castle_queenside = _castling.get_white_queenside();
                home_row = 0;
            } else {
                king = _b_king;
                our_pawns = black_pawns();
                our_knights = squarewise_and(_b_pieces, _knights);
                our_rooks = squarewise_and(_b_pieces, _rooks);
                our_bishops = squarewise_and(_b_pieces, _bishops);
                our_pieces = black_pieces();
                opponent_pieces = white_pieces();
                forward = -1;
                can_castle_kingside = _castling.get_black_kingside();
                can_castle_queenside = _castling.get_black_queenside();
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
                    && !square_under_attack(king, !_w_turn)
                    && !square_under_attack(Square(home_row, 5), !_w_turn)
                    && !square_under_attack(Square(home_row, 6), !_w_turn)
                ) {
                    ply_list.push_back(Ply(king, Square(home_row, 6)));
                }
            }
            if (can_castle_queenside) {
                if (!all.get_square(home_row, 1) && !all.get_square(home_row, 2) && !all.get_square(home_row, 3)
                    && !square_under_attack(king, !_w_turn)
                    && !square_under_attack(Square(home_row, 2), !_w_turn)
                    && !square_under_attack(Square(home_row, 3), !_w_turn)
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
                            if ((_w_turn && r == 1) || (!_w_turn && r == 6)) {
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
                            if (r == (_w_turn ? 4 : 3) && all_en_passant().get_square(_w_turn ? 7 : 0, c + 1)) {
                                add_ply_if_king_not_in_check(&ply_list, Ply(from, to));
                            }
                        }
                        if (c != 0) {
                            to = Square(r + forward, c - 1);
                            if (opponent_pieces.get_square(to)) {
                                add_pawn_plies(&ply_list, from, to);
                            }
                            if (r == (_w_turn ? 4 : 3) && all_en_passant().get_square(_w_turn ? 7 : 0, c - 1)) {
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

            result += _w_turn ? " w " : " b ";

            if (_castling.no_castle_legal()) {
                result += '-';
            } else {
                if (_castling.get_white_kingside()) { result += "K"; };
                if (_castling.get_white_queenside()) { result += "Q"; };
                if (_castling.get_black_kingside()) { result += "k"; };
                if (_castling.get_black_queenside()) { result += "q"; };
            }
            result += ' ';

            if (all_en_passant().empty()) {
                result += "- ";
            } else if (!white_en_passant().empty()) {
                for (int col = 0; col < 8; col++) {
                    if (_pawns.get_square(0, col)) {
                        result += (col + 'a');
                        result += "3 ";
                        break;
                    }
                }
            } else if (!black_en_passant().empty()) {
                for (int col = 0; col < 8; col++) {
                    if (_pawns.get_square(7, col)) {
                        result += (col + 'a');
                        result += "6 ";
                        break;
                    }
                }
            }
            result += std::to_string(_rule_fifty_ply_clock);
            result += ' ';
            result += std::to_string(_move_count);
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
            return _rule_fifty_ply_clock >= 100;
        }

        bool is_insufficient_mating_material() const {
            if (!_rooks.empty() || !_pawns.empty()) {
                return false;
            }

            // three pieces or less, no queens/rooks/pawns
            if ((squarewise_or(_w_pieces, _b_pieces)).count() < 3) {
                return true;
            }

            // multiple knights
            if (!(_knights.empty())) {
                return false;
            }

            // both a light and dark square bishop
            const bool light_bishop = !squarewise_and(_bishops, LIGHT_SQUARES_MASK).empty();
            const bool dark_bishop = !squarewise_and(_bishops, DARK_SQUARES_MASK).empty();
            return !(light_bishop && dark_bishop);
        }
    
    private:
        Bitboard _w_pieces;
        Bitboard _b_pieces;

        Bitboard _pawns;
        Bitboard _rooks; // includes Queens
        Bitboard _knights;
        Bitboard _bishops; // includes Queens
        Square _w_king;
        Square _b_king;

        bool _w_turn;

        Castling _castling;

        int _rule_fifty_ply_clock = 0;
        int _move_count = 1;

        std::list<BoardSignature> board_history;

        char square_to_char(int row, int col) const {
            if (row == _w_king.get_row() && col == _w_king.get_col()) {
                return 'K';
            } else if (white_queens().get_square(row, col)) {
                return 'Q';
            } else if (white_pawns().get_square(row, col)) {
                return 'P';
            } else if (squarewise_and(_w_pieces, _rooks).get_square(row, col)) {
                return 'R';
            } else if (squarewise_and(_w_pieces, _knights).get_square(row, col)) {
                return 'N';
            } else if (squarewise_and(_w_pieces, _bishops).get_square(row, col)) {
                return 'B';
            } else if (row == _b_king.get_row() && col == _b_king.get_col()) {
                return 'k';
            } else if (black_queens().get_square(row, col)) {
                return 'q';
            } else if (black_pawns().get_square(row, col)) {
                return 'p';
            } else if (squarewise_and(_b_pieces, _rooks).get_square(row, col)) {
                return 'r';
            } else if (squarewise_and(_b_pieces, _knights).get_square(row, col)) {
                return 'n';
            } else if (squarewise_and(_b_pieces, _bishops).get_square(row, col)) {
                return 'b';
            } else {
                return '.';
            }
        }

        bool add_ply_if_king_not_in_check(std::vector<Ply>* ply_list, const Ply ply) const {
            Board b = copy_without_history();
            b.apply_ply_without_history(ply);
            Square king = _w_turn ? b._w_king : b._b_king;
            if (!b.square_under_attack(king)) {
                ply_list->push_back(ply);
                return true;
            }
            return false;
        }

        void add_pawn_plies(std::vector<Ply>* ply_list, Square from, Square to) const {
            if (to.get_row() == 7 || to.get_row() == 0) {
                // We don't need to check if the pawn is pinned for each possible promotion
                bool not_pinned = add_ply_if_king_not_in_check(ply_list, Ply(from, to, Ply::Promotion::Queen));
                if (not_pinned) {
                    ply_list->push_back(Ply(from, to, Ply::Promotion::Knight));
                    ply_list->push_back(Ply(from, to, Ply::Promotion::Rook));
                    ply_list->push_back(Ply(from, to, Ply::Promotion::Bishop));
                }
            } else {
                add_ply_if_king_not_in_check(ply_list, Ply(from, to));
            }
        }
};
