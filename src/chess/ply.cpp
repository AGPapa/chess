 #include <cstdint>
 #include <string>
 #include <iostream>
 #include "square.cpp"
 
class Ply {

    public:
        Ply(Square from, Square to) {
            board = 0;
        };

    private:
        std::uint64_t board;
};
