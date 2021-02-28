#include "../../interface/uci.cpp"

#include <gtest/gtest.h>

bool is_bestmove_valid(Board b, std::string output) {
    std::istringstream stream = std::istringstream(output);
    std::string token = "";
    stream >> token;
    if (token != "bestmove") {
        return false;
    }
    stream >> token;
    Ply ply_to_check = Ply(token);
    for (Ply p : b.generate_potential_plies()) {
        if (p == ply_to_check) return true;
    }
    return false;
}

TEST(UCITest, foo) {
    Board b = Board::default_board();
    UCI engine = UCI();

    engine.uci();
    engine.is_ready();
    engine.new_game();

    std::istringstream input_stream = std::istringstream("startpos");
    std::ostringstream output_stream = std::ostringstream();
    engine.position(input_stream);
    engine.is_ready();
    input_stream = std::istringstream("wtime 300000 btime 300000 movestogo 40");
    engine.go(input_stream, output_stream);
    ASSERT_EQ(true, is_bestmove_valid(b, output_stream.str()));

    output_stream = std::ostringstream();
    input_stream = std::istringstream("startpos moves h2h4 d7d5");
    b.apply_ply(Ply("h2h4"));
    b.apply_ply(Ply("d7d5"));
    engine.position(input_stream);
    engine.is_ready();
    input_stream = std::istringstream("wtime 300000 btime 300000 movestogo 39");
    engine.go(input_stream, output_stream);
    ASSERT_EQ(true, is_bestmove_valid(b, output_stream.str()));

    output_stream = std::ostringstream();
    input_stream = std::istringstream("startpos moves h2h4 d7d5 h4h5 e7e5");
    b.apply_ply(Ply("h4h5"));
    b.apply_ply(Ply("e7e5"));
    engine.position(input_stream);
    engine.is_ready();
    input_stream = std::istringstream("wtime 300000 btime 300000 movestogo 38");
    engine.go(input_stream, output_stream);
    ASSERT_EQ(true, is_bestmove_valid(b, output_stream.str()));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}