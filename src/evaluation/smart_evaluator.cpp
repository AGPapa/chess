#pragma once

#include "policy.cpp"
#include "../chess/board.cpp"

class SmartEvaluator {

    public:
        SmartEvaluator() {};

        virtual std::unique_ptr<Policy> evaluate (const Board b, const Board prev_board, const Ply ply, std::vector<Ply> ply_list) {
           return std::unique_ptr<Policy>(new Policy(0));
        }

        virtual std::unique_ptr<Policy> evaluate (const Board b, std::vector<Ply> ply_list) {
            return std::unique_ptr<Policy>(new Policy(0));
        }
};