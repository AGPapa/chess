#include <stdlib.h>
#include <fstream>

#include "smart_evaluator.cpp"

class MockNeuralNet : public SmartEvaluator {

    public:
        MockNeuralNet() {
        };

        std::unique_ptr<Policy> evaluate (const Board b, const Board prev_board, const Ply ply, std::vector<Ply> ply_list) {
           return evaluate(b, ply_list); 
        }

        std::unique_ptr<Policy> evaluate (const Board b, std::vector<Ply> ply_list) {
            float score = (b.white_pawns().count() - b.black_pawns().count()
            + 3*(b.white_bishops().count() - b.black_bishops().count())
            + 3*(b.white_knights().count() - b.black_knights().count())
            + 5*(b.white_rooks().count() - b.black_rooks().count())
            + 9*(b.white_queens().count() - b.black_queens().count()));

            if (b.is_white_turn()) { score += 0.01; }
            else { score -= -0.01; };

            std::unique_ptr<Policy> pol = std::unique_ptr<Policy>(new Policy(score/25.0));

            for (Ply p : ply_list) {
                if (b.all_pieces().get_square(p.to_square())) {
                    pol->add_action(p, 0.2);
                } else {
                    pol->add_action(p, 0.1);
                }
            }

            return pol;
        }
};