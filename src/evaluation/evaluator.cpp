#include "evaluator.hpp"

std::unique_ptr<Policy> Evaluator::evaluate(const Board prev_board, const Ply p) {
    Board b = prev_board;
    b.apply_ply(p);
    
    std::unique_ptr<Policy> policy = _draw(b);
    if (policy != nullptr) return policy;

    std::vector<Ply> ply_list = b.generate_potential_plies();

    policy = _checkmate_or_stalemate(b, ply_list);
    if (policy != nullptr) return policy;

    // TODO: Replace with real implementation
    // return net.evaluate(b.copy_without_history(), prev_board.copy_without_history(), p, ply_list);
    policy = std::unique_ptr<Policy>(new Policy(0.01));

    for (Ply p : ply_list) {
        policy->add_action(p, 0.1);
    }

    return policy;
}

std::unique_ptr<Policy> Evaluator::evaluate(const Board b) {
    std::unique_ptr<Policy> policy = _draw(b);
    if (policy != nullptr) return policy;

    std::vector<Ply> ply_list = b.generate_potential_plies();

    policy = _checkmate_or_stalemate(b, ply_list);
    if (policy != nullptr) return policy;

    // TODO: Replace with real implementation
    // return net.evaluate(b.copy_without_history(), ply_list);
    policy = std::unique_ptr<Policy>(new Policy(0.01));

    for (Ply p : ply_list) {
        policy->add_action(p, 0.1);
    }

    return policy;
}

std::unique_ptr<Policy> Evaluator::_draw(const Board b) {
    std::unique_ptr<Policy> draw_policy = std::unique_ptr<Policy>(new Policy(0));
    if (b.is_fifty_move_draw()) { return draw_policy; }
    if (b.is_insufficient_mating_material()) { return draw_policy; }
    if (b.is_threefold_repetition()) { return draw_policy; }
    return nullptr;
}

std::unique_ptr<Policy> Evaluator::_checkmate_or_stalemate(const Board b, const std::vector<Ply> ply_list) {
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
    return nullptr;
}

NeuralNet Evaluator::net = NeuralNet();