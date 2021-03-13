#include <sstream>
#include <string>

#include "../search/searcher.cpp"

class UCI {

    public:
        UCI() {};

        bool parse_line(std::string line, std::ostream& output) {
            std::istringstream input_stream = std::istringstream(line);
            std::string token;
            input_stream >> token;

            if (token == "uci") {
                output << _uci();
            } else if (token == "isready") {
                output << _is_ready();
            } else if (token == "ucinewgame") {
                _new_game();
            } else if (token == "position") {
                _position(input_stream);
            } else if (token == "go") {
                _go(input_stream, output);
            } else if (token == "stop") {
                output << _stop();
               return true;
            }
            return false;
        }

    private:
        std::unique_ptr<Searcher> _searcher;
        Ply _best_ply;
        int _num_moves;

        std::string _uci() {
            return "id name TBDEngineName\nid author Antonio Papa, Viveque Ramji\nuciok\n";  
        }

        void _position(std::istringstream& input) {
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
                        _searcher->apply_ply(Ply(token));
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

        void _new_game() {
            _searcher = std::unique_ptr<Searcher>(new Searcher());
            _best_ply = Ply();
            _num_moves = 0;
        }

        void _go(std::istringstream& input, std::ostream& output) {
            int w_time = 0;
            int b_time = 0;
            int moves_to_next_time_control = 0;
            int w_inc = 0;
            int b_inc = 0;
            std::string token = "";

            input >> token;
            while (token != "") {
                if (token == "wtime") {
                    input >> token;
                    w_time = std::stoi( token );
                } else if (token == "btime") {
                    input >> token;
                    b_time = std::stoi( token );
                } else if (token == "movestogo") {
                    input >> token;
                    moves_to_next_time_control = std::stoi( token );
                } else if (token == "winc") {
                    input >> token;
                    w_inc = std::stoi( token );
                } else if (token == "binc") {
                    input >> token;
                    b_inc = std::stoi( token );
                } else {
                    throw std::runtime_error("Bad go command - unsuported option " + token + " detected");
                }
                token = "";
                input >> token;
            }


            _searcher->start_searching();
            std::this_thread::sleep_for (std::chrono::milliseconds(100));
            _searcher->stop_searching();

            _best_ply = _searcher->find_best_ply();

            output << "bestmove " + _best_ply.to_string() + '\n';
        }

        std::string _is_ready() {
            return "readyok\n";
        }

        std::string _stop() {
            return "bestmove " + _best_ply.to_string() + '\n';
        }
};