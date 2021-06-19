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

TEST(UCITest, integration) {
    Board b = Board::default_board();
    std::ostringstream out = std::ostringstream();
    UCI engine = UCI(out);

    engine.parse_line("uci");
    engine.parse_line("isready");
    engine.parse_line("ucinewgame");

    engine.parse_line("position startpos");
    engine.parse_line("isready");
    out.str("");
    engine.parse_line("go wtime 300000 btime 300000 movestogo 40");
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    engine.parse_line("stop");
    ASSERT_EQ(true, is_bestmove_valid(b, out.str()));

    out = std::ostringstream();
    b.apply_ply(Ply("h2h4"));
    b.apply_ply(Ply("d7d5"));
    engine.parse_line("position startpos moves h2h4 d7d5");
    engine.parse_line("isready");
    out.str("");
    engine.parse_line("go wtime 300000 btime 300000 movestogo 39");
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    engine.parse_line("stop");
    ASSERT_EQ(true, is_bestmove_valid(b, out.str()));

    b.apply_ply(Ply("h4h5"));
    b.apply_ply(Ply("e7e5"));
    engine.parse_line("position startpos moves h2h4 d7d5 h4h5 e7e5");
    engine.parse_line("isready");
    out.str("");
    engine.parse_line("go wtime 300000 btime 300000 movestogo 38");
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    engine.parse_line("stop");
    ASSERT_EQ(true, is_bestmove_valid(b, out.str()));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}