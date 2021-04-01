#include "../chess/ply.cpp"

class Action {
    public:

        Action(Ply ply, float probability) {
            _ply = ply;
            _probability = probability;
        }

        const Ply ply() {
            return _ply;
        }

        const float probability() {
            return _probability;
        }
    
    private:
        Ply _ply;
        float _probability;
};