#include "evaluator.hpp"

std::unique_ptr<Policy> Evaluator::evaluate(const Board b) {
    std::unique_ptr<Policy> draw_policy = std::unique_ptr<Policy>(new Policy(0));
    if (b.is_fifty_move_draw()) { return draw_policy; }
    if (b.is_insufficient_mating_material()) { return draw_policy; }
    if (b.is_threefold_repetition()) { return draw_policy; }

    std::vector<Ply> ply_list = b.generate_potential_plies();

    // checkmate or stalemate
    if (ply_list.empty()) {
        float result = 0;
        if (b.is_white_turn()) {
            if (b.is_white_king_in_check()) { result = -1; }
        } else {
            if (b.is_black_king_in_check()) { result = 1; }
        }
        std::unique_ptr<Policy> checkmate_policy = std::unique_ptr<Policy>(new Policy(result));
        return checkmate_policy;
    }

    // TODO: Replace with real implementation
    // return net.evaluate(b, ply_list);
    std::unique_ptr<Policy> policy = std::unique_ptr<Policy>(new Policy(0.01));

    for (Ply p : ply_list) {
        policy->add_action(p, 0.1);
    }

    return policy;
}

NeuralNet Evaluator::net = NeuralNet();