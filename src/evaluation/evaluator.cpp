#pragma once

#include "evaluator.hpp"
#include "neural_net.cpp"
#include "mock_neural_net.cpp"

std::unique_ptr<Policy> Evaluator::evaluate(const Board prev_board, const Ply p) {
    Board b = prev_board;
    b.apply_ply_without_history(p);
    
    std::unique_ptr<Policy> policy = _draw(b);
    if (policy != nullptr) return policy;

    std::vector<Ply> ply_list = b.generate_potential_plies();

    policy = _checkmate_or_stalemate(b, ply_list);
    if (policy != nullptr) return policy;

    return net->evaluate(b, prev_board, p, ply_list);
}

std::unique_ptr<Policy> Evaluator::evaluate(const Board b) {
    std::unique_ptr<Policy> policy = _draw(b);
    if (policy != nullptr) return policy;

    std::vector<Ply> ply_list = b.generate_potential_plies();

    policy = _checkmate_or_stalemate(b, ply_list);
    if (policy != nullptr) return policy;

    return net->evaluate(b, ply_list);
}

std::unique_ptr<Policy> Evaluator::_draw(const Board b) {
    std::unique_ptr<Policy> draw_policy = std::unique_ptr<Policy>(new Policy(0));
    if (b.is_fifty_move_draw()) { return draw_policy; }
    if (b.is_insufficient_mating_material()) { return draw_policy; }
    // TODO - Refactor this so the draw checks all happen in search_job
    // if (b.is_threefold_repetition()) { return draw_policy; }
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

std::unique_ptr<SmartEvaluator> Evaluator::net = std::unique_ptr<SmartEvaluator>(new MockNeuralNet());
//std::unique_ptr<SmartEvaluator> Evaluator::net = std::unique_ptr<SmartEvaluator>(new NeuralNet());
