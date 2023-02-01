#include "board_signature.cpp"

class BoardHistory {

    public:

        BoardHistory() { _index = 0; };

        void clear() { _index = 0; }

        void add(BoardSignature s) { 
            _history[_index] = s;
            _index++;
        }

        bool is_threefold_repetition(BoardSignature current_board) const {
            if (_index < 8) return false;
            int repetition_count = 1;
            // jumps two board signatures at a time to skip white and black turns
            for (int i = _index - 2; i >= 0; i--, i--) {
                if (current_board.is_repetition(_history[i])) {
                    if (repetition_count == 2) return true;
                    i--; i--; // skips two board signatures after we find a repetition
                    repetition_count++;
                }
            }
            return false;
        }

    private:
        BoardSignature _history[50];
        int _index;
};