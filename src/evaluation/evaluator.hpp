#include "neural_net.cpp"
#include "../chess/board.cpp"

class Evaluator {

    public:
        static std::unique_ptr<Policy> evaluate(Board b);

        static NeuralNet net;
};