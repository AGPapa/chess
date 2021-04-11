#include "bitboard.cpp"
#include "castling.cpp"

class BoardSignature {
// tracks the board for repetetion
// doesn't track pawns since history is cleared after a pawn push
// doesn't track whose turn, checked in board by skipping every other element in the history

    public:

        BoardSignature(Bitboard w_p, Bitboard b_p, Bitboard b, Bitboard r, Bitboard n,
            Square w_k, Square b_k, Castling c) {
            w_pieces = w_p;
            b_pieces = b_p;
            bishops = b;
            rooks = r;
            knights = n;

            w_king = w_k;
            b_king = b_k;

            castling = c;
        };

        bool is_repetition(const BoardSignature b) const {
            return b.w_pieces == w_pieces
                && b.b_pieces == b_pieces
                && b.rooks == rooks
                && b.knights == knights
                && b.bishops == bishops
                && b.w_king == w_king
                && b.b_king == b_king
                && b.castling == castling;
        }

    private:
        // TODO: Replace this with a hash

        Bitboard w_pieces;
        Bitboard b_pieces;

        Bitboard rooks;
        Bitboard knights;
        Bitboard bishops;
        Square w_king;
        Square b_king;

        Castling castling;
};