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

            return policy;
        }
};