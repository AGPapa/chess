#include <sstream>
#include <string>

#include "../search/searcher.cpp"

class UCI {

    public:
        UCI() {};

        std::string uci() {
            return "id name TBDEngineName\nid author Antonio Papa, Viveque Ramji\nuciok\n";  
        }

        void position(std::istringstream& input) {
            std::string token;
            input >> token;

            if (token != "startpos") {
                throw std::runtime_error("Bad position command - only startpos is currently supported");
            }

            token = "";
            input >> token;

            if (token.empty()) {
                return;
            } else if (token == "moves") {
                input >> token;
                int input_move_count = 0;
                while (!token.empty()) {
                    if (input_move_count >= _num_moves) {
                        _searcher.apply_ply(Ply(token));
                        _num_moves++;
                    }
                    input_move_count++;
                    token = "";
                    input >> token;
                }
            } else {
                throw std::runtime_error("Bad position command - string other than moves detected");
            }
        }

        void new_game() {
            _searcher = Searcher();
            _best_ply = Ply();
            _num_moves = 0;
        }

        void go(std::istringstream& input, std::ostream& output) {
            // TODO: support additional parameters
            std::string token;
            input >> token;

            _searcher.start_searching();
            std::this_thread::sleep_for (std::chrono::milliseconds(100));
            _searcher.stop_searching();

            _best_ply = _searcher.find_best_ply();

            output << "bestmove " + _best_ply.to_string() + '\n';
        }

        std::string is_ready() {
            return "readyok\n";
        }

        std::string stop() {
            return "bestmove " + _best_ply.to_string() + '\n';
        }

    private:
        Searcher _searcher;
        Ply _best_ply;
        int _num_moves;
};