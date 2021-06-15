#include "evaluator.hpp"

Policy Evaluator::evaluate(Board b) {
    if (b.is_fifty_move_draw()) { return Policy(0); }
    if (b.is_insufficient_mating_material()) { return Policy(0); }
    if (b.is_threefold_repetition()) { return Policy(0); }

    std::vector<Ply> ply_list = b.generate_potential_plies();

    // checkmate or stalemate
    if (ply_list.empty()) {
        float result = 0;
        if (b.is_white_turn()) {
            if (b.is_white_king_in_check()) { result = -1; }
        } else {
            if (b.is_black_king_in_check()) { result = 1; }
        }
        return Policy(result);
    }

    // TODO: Replace with real implementation
    // return net.evaluate(b, ply_list);
    Policy policy = Policy(0.01);

    for (Ply p : ply_list) {
        policy.add_action(p, 0.1);
    }

    return policy;
}

NeuralNet Evaluator::net = NeuralNet();