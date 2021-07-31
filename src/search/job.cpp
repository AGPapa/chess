#pragma once

#include "../chess/board.cpp"

class Job {
    public:
        enum QueueType { tree, evaluate, backprop, invalid };
        virtual std::unique_ptr<Job> run() { return nullptr; };
        virtual QueueType type() { return invalid; };
};