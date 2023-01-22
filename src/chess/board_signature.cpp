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
            std::hash<uint64_t> hasher;
            _seed = hasher(w_p.get_int());
            _seed ^= hasher(b_p.get_int()) + 0x9e3779b9 + (_seed<<6) + (_seed>>2);
            _seed ^= hasher(b.get_int()) + 0x9e3779b9 + (_seed<<6) + (_seed>>2);
            _seed ^= hasher(r.get_int()) + 0x9e3779b9 + (_seed<<6) + (_seed>>2);
            _seed ^= hasher(n.get_int()) + 0x9e3779b9 + (_seed<<6) + (_seed>>2);
            _seed ^= hasher(c.get_int() + (w_k.get_int()<<8) + (b_k.get_int()<<16)) + 0x9e3779b9 + (_seed<<6) + (_seed>>2);
        };

        bool is_repetition(const BoardSignature b) const {
            return b._seed == _seed;
        }

    private:
        std::size_t _seed;
};