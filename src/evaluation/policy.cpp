#pragma once

#include <vector>

#include "action.cpp"

class Policy {

    public:

        Policy(float value) {
            _value = value;
            _num_actions = 0;
        }

        void add_action(Ply p, float probability) {
            _actions[_num_actions] = Action(p, probability);
            _num_actions++;
        }

        void set_value(float value) {
            _value = value;
        }

        float _value;
        short _num_actions;
        Action _actions[218]; // 218 is the most number of chess moves from one position
};