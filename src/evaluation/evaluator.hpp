#pragma once

#include "smart_evaluator.cpp"
#include "../chess/board.cpp"

class Evaluator {

    public:
        static std::unique_ptr<Policy> evaluate(Board curr_board, Board prev_board, Ply p);

        static std::unique_ptr<Policy> evaluate(Board b);

        static std::unique_ptr<SmartEvaluator> net;

    private:
        static std::unique_ptr<Policy> _draw(const Board b);

        static std::unique_ptr<Policy> _checkmate_or_stalemate(const Board b, const std::vector<Ply> ply_list);

};