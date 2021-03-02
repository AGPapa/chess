#include <iostream>
#include <sstream>
#include <string>
#include "uci.cpp"

int main(int argc, char** argv) {
    std::string line;
    UCI engine = UCI();
    do {
        if (getline(std::cin, line)) {

            std::istringstream line_stream = std::istringstream(line);
            std::string token;
            line_stream >> token;

            if (token == "uci") {
                std::cout << engine.uci();
            } else if (token == "isready") {
                std::cout << engine.is_ready();
            } else if (token == "ucinewgame") {
                engine.new_game();
            } else if (token == "position") {
                engine.position(line_stream);
            } else if (token == "go") {
                engine.go(line_stream, std::cout);
            } else if (token == "stop") {
                std::cout << engine.stop();
                break;
            }
        } else {
            break;
        }
    } while(true);
    return 0;
};