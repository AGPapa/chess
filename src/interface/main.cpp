#include <iostream>
#include <sstream>
#include <string>
#include "uci.cpp"

int main(int argc, char** argv) {
    std::string line;
    UCI engine = UCI(std::cout);
    do {
        if (getline(std::cin, line)) {
            std::istringstream line_stream = std::istringstream(line);
            bool stop = engine.parse_line(line);
            if (stop) { break; }
        } else {
            break;
        }
    } while(true);
    return 0;
};