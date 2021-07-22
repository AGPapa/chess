#include <cstdint>

#include "layer.cpp"
#include "cache.cpp"

#if defined(USE_NEON)
#include <arm_neon.h>
#endif

template<int OutputDimension>
class TransformationLayer : public Layer<std::int16_t> {

    public:
        static const int SQUARES = 64;
        static const int PIECES = 6;
        static const int SIDES = 2;
        static const int PIECE_RELATIONS = SQUARES*PIECES*SIDES;
        static const int CASTLING = 4;
        static const int INPUT_DIMENSION = (PIECE_RELATIONS + CASTLING);

        struct Weights {
            std::int8_t weights[INPUT_DIMENSION * OutputDimension];
            std::int8_t biases[OutputDimension];
        };

        TransformationLayer(Cache<int, Weights>* cache, std::unique_ptr<Weights>(*load_weights)(int), const bool flipped) {
            _flipped = flipped;
            _cache = cache;
            _load_weights = load_weights;
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

            std::int8_t input[INPUT_DIMENSION] = {0};

            for (int i = 0; i < SQUARES; i++) {
                if (all_pieces.get_square(i)) {
                    input[i] = friendly_pawns.get_square(i);
                    input[SQUARES*1 + i] = friendly_knights.get_square(i);
                    input[SQUARES*2 + i] = friendly_bishops.get_square(i);
                    input[SQUARES*3 + i] = friendly_rooks.get_square(i);
                    input[SQUARES*4 + i] = friendly_queens.get_square(i);
                    input[SQUARES*5 + i] = enemy_pawns.get_square(i);
                    input[SQUARES*6 + i] = enemy_knights.get_square(i);
                    input[SQUARES*7 + i] = enemy_bishops.get_square(i);
                    input[SQUARES*8 + i] = enemy_rooks.get_square(i);
                    input[SQUARES*9 + i] = enemy_queens.get_square(i);
                }
            }
            input[SQUARES*10 + enemy_king] = 1;
            input[PIECE_RELATIONS] = friendly_kingside_castle;
            input[PIECE_RELATIONS + 1] = friendly_queenside_castle;
            input[PIECE_RELATIONS + 2] = enemy_kingside_castle;
            input[PIECE_RELATIONS + 3] = enemy_queenside_castle;

            Weights *weights_ptr = _cache->get(friendly_king);
            std::int8_t *weights;
            std::int8_t *biases;
            if (weights_ptr != nullptr) {
                biases = weights_ptr->biases;
                weights = weights_ptr->weights;
            } else {
                _cache->add(friendly_king, std::move(_load_weights(friendly_king)));
                weights_ptr = _cache->get(friendly_king);
                biases = weights_ptr->biases;
                weights = weights_ptr->weights;
            }

            // applies weights
            for (int i = 0; i < OutputDimension; i++) {
                int j = 0;
                std::int16_t sum = biases[i];
                #if defined(USE_NEON)
                    const short transfer_size = 8;
                    short segments = INPUT_DIMENSION / transfer_size;
                    j = segments * transfer_size;

                    int16x8_t sums_vector = {0};
                    std::int16_t sums[transfer_size];
                    for(short x = 0; x < segments; x++) {
                        short offset = x * transfer_size;
                        int8x8_t input_vector = vld1_s8(input + offset); // load vector elements to registers
                        int8x8_t weights_vector = vld1_s8(weights + i * INPUT_DIMENSION + offset); // load vector elements to registers
                   
                        sums_vector = vmlal_s8(sums_vector, input_vector, weights_vector); // sums + (input dot weights)
                    }
                    vst1q_s16(sums, sums_vector); // store vector elements in memory
                    sum += sums[0] + sums[1] + sums[2] + sums[3] + sums[4] + sums[5] + sums[6] + sums[7];
                #endif

                for (; j < INPUT_DIMENSION; j++) {
                    sum += weights[i * INPUT_DIMENSION + j] * input[j];
                }
                output[i] = sum;
            }
        }

        const int output_dimension() const {
            return OutputDimension;
        }

    private:
        Cache<int, Weights>* _cache;
        bool _flipped;
        std::unique_ptr<Weights>(*_load_weights)(int);
};