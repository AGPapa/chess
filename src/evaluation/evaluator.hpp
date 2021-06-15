#include "neural_net.cpp"
#include "../chess/board.cpp"

class Evaluator {

    public:
        static Policy evaluate(Board b);

        static NeuralNet net;
};