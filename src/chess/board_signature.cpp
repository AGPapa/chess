#include "bitboard.cpp"
#include "castling.cpp"

class BoardSignature {
// tracks the board for repetetion
// doesn't track pawns since history is cleared after a pawn push
// doesn't track whose turn, checked in board by skipping every other element in the history

    public:

        BoardSignature() {};

        BoardSignature(Bitboard w_p, Bitboard b_p, Bitboard b, Bitboard r, Bitboard n,
            Square w_k, Square b_k, Castling c) {
            _w_pieces = w_p;
            _b_pieces = b_p;
            _bishops = b;
            _rooks = r;
            _knights = n;

            _w_king = w_k;
            _b_king = b_k;

            _castling = c;
        };

        bool is_repetition(const BoardSignature b) const {
            return b._w_pieces == _w_pieces
                && b._b_pieces == _b_pieces
                && b._rooks == _rooks
                && b._knights == _knights
                && b._bishops == _bishops
                && b._w_king == _w_king
                && b._b_king == _b_king
                && b._castling == _castling;
        }

    private:
        // TODO: Replace this with a hash

        Bitboard _w_pieces;
        Bitboard _b_pieces;

        Bitboard _rooks;
        Bitboard _knights;
        Bitboard _bishops;
        Square _w_king;
        Square _b_king;

        Castling _castling;
};