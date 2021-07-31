#include "expanded_node.cpp"
#include "job.cpp"

class BackpropJob : public Job {

    public:
        BackpropJob(float s, std::unique_ptr<std::vector<ExpandedNode*>> l, bool w_turn) {
            _score = s;
            _lineage = std::move(l);
            _is_white_turn = w_turn;
        }

        std::unique_ptr<Job> run() {
            bool white_to_play = _is_white_turn;
            for (std::vector<ExpandedNode*>::reverse_iterator node = _lineage->rbegin(); node != _lineage->rend(); ++node ) {
                (*node)->_visits += 1;
                if (white_to_play) { // potential improvement: skip this part for draws
                    (*node)->_score -= _score;
                } else {
                    (*node)->_score += _score;
                }
                white_to_play = !white_to_play;
            }
            return nullptr;
        }

        QueueType type() {
            return Job::backprop;
        }

    private:
        float _score;
        std::unique_ptr<std::vector<ExpandedNode*>> _lineage;
        bool _is_white_turn;

};