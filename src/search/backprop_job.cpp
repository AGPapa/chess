#include "node.cpp"

class BackpropJob {

    public:
        BackpropJob() {}

        BackpropJob(float s, std::shared_ptr<std::vector<Node*>> l, bool w_turn) {
            _score = s;
            _lineage = l;
            _is_white_turn = w_turn;
        }

        void run() {
            bool white_to_play = _is_white_turn;
            for (std::vector<Node*>::reverse_iterator node = _lineage->rbegin(); node != _lineage->rend(); ++node ) {
                if (white_to_play) { // potential improvement: skip this part for draws
                    (*node)->_score -= _score;
                } else {
                    (*node)->_score += _score;
                }
                (*node)->_score += 1; // undoes virtual loss
                white_to_play = !white_to_play;
            }
        }

    private:
        float _score;
        std::shared_ptr<std::vector<Node*>> _lineage;
        bool _is_white_turn;

};