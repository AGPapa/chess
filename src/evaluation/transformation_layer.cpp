#include <cstdint>
#include <vector>
#include <map>

#include "layer.cpp"

#if defined(USE_NEON)
#include <arm_neon.h>
#endif

template<int OutputDimension>
class TransformationLayer : public Layer<std::int16_t> {

    public:
        static const int SQUARES = 64;
        static const int PIECES = 6;
        static const int SIDES = 2;
        static const int PIECE_RELATIONS = SQUARES*(PIECES*SIDES-1);
        static const int CASTLING = 4;
        static const int INPUT_DIMENSION = (PIECE_RELATIONS + CASTLING);

        struct Weights {
            std::int8_t weights[INPUT_DIMENSION * OutputDimension];
            std::int8_t biases[OutputDimension];
        };

        TransformationLayer() {}

        TransformationLayer(std::map<int, std::unique_ptr<Weights>>* weights, const bool flipped) {
            _flipped = flipped;
            _weights = weights;
        }

        void propagate(const Board b, std::int16_t* output) {
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
            if (b.is_white_turn() ^ _flipped) {
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

            int king_offset = friendly_king * PIECE_RELATIONS;

            Bitboard all_pieces;
            if (_flipped) {
                all_pieces = b.all_pieces().mirror();
            } else {
                all_pieces = b.all_pieces();
            }

            std::vector<int> inputs;
            inputs.reserve(33);

            for (int i = 0; i < SQUARES; i++) {
                if (all_pieces.get_square(i)) {
                    if (friendly_pawns.get_square(i)) { inputs.push_back(i); };
                    if (friendly_knights.get_square(i)) { inputs.push_back(SQUARES*1 + i); };
                    if (friendly_bishops.get_square(i)) { inputs.push_back(SQUARES*2 + i); };
                    if (friendly_rooks.get_square(i)) { inputs.push_back(SQUARES*3 + i); };
                    if (friendly_queens.get_square(i)) { inputs.push_back(SQUARES*4 + i); };
                    if (enemy_pawns.get_square(i)) { inputs.push_back(SQUARES*5 + i); };
                    if (enemy_knights.get_square(i)) { inputs.push_back(SQUARES*6 + i); };
                    if (enemy_bishops.get_square(i)) { inputs.push_back(SQUARES*7 + i); };
                    if (enemy_rooks.get_square(i)) { inputs.push_back(SQUARES*8 + i); };
                    if (enemy_queens.get_square(i)) { inputs.push_back(SQUARES*9 + i); };
                }
            }
            inputs.push_back(SQUARES*10 + enemy_king);
            if (friendly_kingside_castle) { inputs.push_back(PIECE_RELATIONS + 0); };
            if (friendly_queenside_castle) { inputs.push_back(PIECE_RELATIONS + 1); };
            if (enemy_kingside_castle) { inputs.push_back(PIECE_RELATIONS + 2); };
            if (enemy_queenside_castle) { inputs.push_back(PIECE_RELATIONS + 3); };

            Weights *weights_ptr = _weights->at(friendly_king).get();
            std::int8_t *weights = weights_ptr->weights;
            std::int8_t *biases  = weights_ptr->biases;

            /*
                weights are saved in different order than in other layers
                input 0 - [output 0, output 1, output 2]
                input 1 - [output 0, output 1, output 2]
            */

            int i = 0;

            #if defined(USE_NEON)
                const short transfer_size = 8;
                short segments = OutputDimension / transfer_size;
                i = segments * transfer_size;
                for (int x = 0; x < segments; x++) {
                    std::int16_t sums[transfer_size];
                    short offset = x * transfer_size;

                    int16x8_t sums_vector = {0};
                    for (int j : inputs) {
                        int8x8_t weights_vector = vld1_s8(weights + j * OutputDimension + offset); // load vector elements to registers
                        sums_vector = vaddw_s8(sums_vector, weights_vector); // sums += weights (don't need weights dot inputs because input is 1)
                    }
                    vst1q_s16(sums, sums_vector); // store vector elements in memory

                    for (int y = 0; y < transfer_size; y++) {
                        output[x * transfer_size + y] = sums[y] + biases[x * transfer_size + y];
                    }
                }
            #endif

            for (; i < OutputDimension; i++) {
                output[i] = biases[i];
                for (int j : inputs) {
                   output[i] += weights[j * OutputDimension + i];
                }
            }
        }

        const int output_dimension() const {
            return OutputDimension;
        }

    private:
        bool _flipped;
        std::map<int, std::unique_ptr<Weights>>* _weights;
};