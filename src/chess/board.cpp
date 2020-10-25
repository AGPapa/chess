 #include <cstdint>
 #include <string>
 #include <iostream>
 #include <sstream>
 #include "bitboard.cpp"
 #include "castling.cpp"


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

        std::string to_fen() const {
            // TODO: Add 50 move rule, castling, en-passant
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
            result += (rule_fifty_ply_clock + '0');
            result += ' ';
            result += (move_count + '0');
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
};
