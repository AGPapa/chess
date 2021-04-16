#include "policy.cpp"
#include "../chess/board.cpp"

class Evaluator {

    public:
        static Policy evaluate(Board b) {
            if (b.is_threefold_repetition()) { return Policy(0); }

            // TODO: Replace with real implementation
            Policy policy = Policy(0.01);

            for (Ply p : b.generate_potential_plies()) {
                policy.add_action(p, 0.1);
            }

            // checkmate or stalemate
            if (policy.actions().empty()) {
                float result = 0;
                if (b.is_white_turn()) {
                    if (b.is_white_king_in_check()) { result = -1; }
                } else {
                    if (b.is_black_king_in_check()) { result = 1; }
                }
                policy.set_value(result);
            }

            return policy;
        }
};