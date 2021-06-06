#include <cstdint>

#include "layer.cpp"

static const int SQUARES = 64;
static const int PIECES = 5;
static const int SIDES = 2;
static const int PIECE_RELATIONS = SQUARES*PIECES*SIDES;
static const int CASTLING = 4;
static const int OUTPUT_DIMENSION = (SQUARES*PIECE_RELATIONS + CASTLING) * SIDES;

class TransformationLayer : public Layer<std::int8_t> {

    public:

        TransformationLayer() {}

        const void propagate(const Board b, std::int8_t* output) {
            int friendly_king;
            int enemy_king;
            Bitboard friendly_pawns;
            Bitboard friendly_knights;
            Bitboard friendly_bishops;
            Bitboard friendly_rooks;
            Bitboard friendly_queens;
            bool friendly_kingside_castle;
            bool friendly_queenside_castle;
            Bitboard enemy_pawns;
            Bitboard enemy_knights;
            Bitboard enemy_bishops;
            Bitboard enemy_rooks;
            Bitboard enemy_queens;
            bool enemy_kingside_castle;
            bool enemy_queenside_castle;
            if (b.is_white_turn()) {
                friendly_king = b.white_king().get_int_value();
                friendly_pawns = b.white_pawns();
                friendly_knights = b.white_knights();
                friendly_bishops = b.white_bishops();
                friendly_rooks = b.white_rooks();
                friendly_queens = b.white_queens();
                friendly_kingside_castle = b.can_white_castle_kingside();
                friendly_queenside_castle = b.can_white_castle_queenside();

                enemy_king = b.black_king().get_int_value();
                enemy_pawns = b.black_pawns();
                enemy_knights = b.black_knights();
                enemy_bishops = b.black_bishops();
                enemy_rooks = b.black_rooks();
                enemy_queens = b.black_queens();
                enemy_kingside_castle = b.can_black_castle_kingside();
                enemy_queenside_castle = b.can_black_castle_queenside();
            } else {
                friendly_king = b.black_king().mirror().get_int_value();
                friendly_pawns = b.black_pawns().mirror();
                friendly_knights = b.black_knights().mirror();
                friendly_bishops = b.black_bishops().mirror();
                friendly_rooks = b.black_rooks().mirror();
                friendly_queens = b.black_queens().mirror();
                friendly_kingside_castle = b.can_black_castle_kingside();
                friendly_queenside_castle = b.can_black_castle_queenside();

                enemy_king = b.white_king().mirror().get_int_value();
                enemy_pawns = b.white_pawns().mirror();
                enemy_knights = b.white_knights().mirror();
                enemy_bishops = b.white_bishops().mirror();
                enemy_rooks = b.white_rooks().mirror();
                enemy_queens = b.white_queens().mirror();
                enemy_kingside_castle = b.can_white_castle_kingside();
                enemy_queenside_castle = b.can_white_castle_queenside();
            }

            int friendly_king_offset = friendly_king * PIECE_RELATIONS;
            int enemy_king_offset = enemy_king * PIECE_RELATIONS + (SQUARES * PIECE_RELATIONS + CASTLING);

            Bitboard all_pieces = b.all_pieces();

            for (int i = 0; i < SQUARES; i++) {
                //TODO: figure out where opponent king goes
                if (all_pieces.get_square(i)) {
                    output[friendly_king_offset + i] = friendly_pawns.get_square(Square(i));
                    output[friendly_king_offset + SQUARES*1 + i] = friendly_knights.get_square(i);
                    output[friendly_king_offset + SQUARES*2 + i] = friendly_bishops.get_square(i);
                    output[friendly_king_offset + SQUARES*3 + i] = friendly_rooks.get_square(i);
                    output[friendly_king_offset + SQUARES*4 + i] = friendly_queens.get_square(i);
                    output[friendly_king_offset + SQUARES*5 + i] = enemy_pawns.get_square(i);
                    output[friendly_king_offset + SQUARES*6 + i] = enemy_knights.get_square(i);
                    output[friendly_king_offset + SQUARES*7 + i] = enemy_bishops.get_square(i);
                    output[friendly_king_offset + SQUARES*8 + i] = enemy_rooks.get_square(i);
                    output[friendly_king_offset + SQUARES*9 + i] = enemy_queens.get_square(i);
                }
                if (all_pieces.mirror().get_square(i)) {
                    output[enemy_king_offset + i] = enemy_pawns.get_square(i);
                    output[enemy_king_offset + SQUARES*1 + i] = enemy_knights.get_square(i);
                    output[enemy_king_offset + SQUARES*2 + i] = enemy_bishops.get_square(i);
                    output[enemy_king_offset + SQUARES*3 + i] = enemy_rooks.get_square(i);
                    output[enemy_king_offset + SQUARES*4 + i] = enemy_queens.get_square(i);
                    output[enemy_king_offset + SQUARES*5 + i] = friendly_pawns.get_square(i);
                    output[enemy_king_offset + SQUARES*6 + i] = friendly_knights.get_square(i);
                    output[enemy_king_offset + SQUARES*7 + i] = friendly_bishops.get_square(i);
                    output[enemy_king_offset + SQUARES*8 + i] = friendly_rooks.get_square(i);
                    output[enemy_king_offset + SQUARES*9 + i] = friendly_queens.get_square(i);
                }
            }
            output[40960] = friendly_kingside_castle;
            output[40961] = friendly_queenside_castle;
            output[40962] = enemy_kingside_castle;
            output[40963] = enemy_queenside_castle;

            output[81924] = enemy_kingside_castle;
            output[81925] = enemy_queenside_castle;
            output[81926] = friendly_kingside_castle;
            output[81927] = friendly_queenside_castle;
        }

        constexpr int output_dimension() const {
            return OUTPUT_DIMENSION;
        }

    private:
        std::int16_t* _output;
};