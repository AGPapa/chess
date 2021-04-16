#include <vector>

#include "action.cpp"

class Policy {

    public:

        Policy(float value) {
            _value = value;
            _action_list = std::vector<Action>();
        }

        void add_action(Ply p, float probability) {
            _action_list.push_back(Action(p, probability));
        }

        void set_value(float value) {
            _value = value;
        }

        const float value() {
            return _value;
        }

        std::vector<Action> actions() {
            return _action_list;
        }

    private:
        float _value;
        std::vector<Action> _action_list;
};