#include "../../search/searcher.cpp"

#include <gtest/gtest.h>
#include <chrono>

TEST(SearcherTest, find_best_ply) {
    std::ostringstream out = std::ostringstream();
    std::unique_ptr<Searcher> s = std::unique_ptr<Searcher>(new Searcher(&out));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    Ply p = s->find_best_ply();
    ASSERT_EQ(p.from_square().get_row() < 2, true);
}

TEST(SearcherTest, apply_ply) {
    std::ostringstream out = std::ostringstream();
    std::unique_ptr<Searcher> s = std::unique_ptr<Searcher>(new Searcher(&out));
    s->apply_ply(Ply("e2e4"));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    Ply p = s->find_best_ply();
    ASSERT_EQ(p.from_square().get_row() > 5, true);

    s = std::unique_ptr<Searcher>(new Searcher(&out));
    s->apply_ply(Ply("e2e4"));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    p = s->find_best_ply();
    ASSERT_EQ(p.from_square().get_row() > 5, true);
}

TEST(SearcherTest, initial_board_constructor) {
    std::ostringstream out = std::ostringstream();
    Board b = Board("7k/7p/8/8/8/8/P7/K7 b - - 0 100");
    std::unique_ptr<Searcher> s = std::unique_ptr<Searcher>(new Searcher(b, &out));;
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    Ply p = s->find_best_ply();
    ASSERT_EQ(p.from_square().get_row() > 5, true);
}

TEST(SearcherTest, checkmate_in_one) {
    std::ostringstream out = std::ostringstream();

    Board a = Board("k7/pp3r2/8/8/8/8/2R3PP/7K w - - 0 100");
    std::unique_ptr<Searcher> s = std::unique_ptr<Searcher>(new Searcher(a, &out));;

    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
    s->stop_searching();
    Ply p = s->find_best_ply();
    ASSERT_EQ(p, Ply("c2c8"));

    s->apply_ply(Ply("c2c3"));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
    s->stop_searching();
    p = s->find_best_ply();
    ASSERT_EQ(p, Ply("f7f1"));

    Board b = Board("k7/1br5/8/8/7n/8/5PPP/7K w - - 0 100");
    s = std::unique_ptr<Searcher>(new Searcher(b, &out));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
    s->stop_searching();
    p = s->find_best_ply();
    ASSERT_EQ(p, Ply("h2h3"));

    Board c = Board("7k/5ppp/8/7N/8/8/1BR5/K7 b - - 0 100");
    s = std::unique_ptr<Searcher>(new Searcher(c, &out));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
    s->stop_searching();
    p = s->find_best_ply();
    ASSERT_EQ(p, Ply("h7h6"));

    Board d = Board("K7/3q4/2k5/8/8/8/8/8 b - - 0 100");
    s = std::unique_ptr<Searcher>(new Searcher(d, &out));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
    s->stop_searching();
    p = s->find_best_ply();
    ASSERT_EQ(p, Ply("d7b7"));
}


TEST(SearcherTest, checkmate_in_two) {
    std::ostringstream out = std::ostringstream();

    Board a = Board("8/2K5/8/2k5/2b5/2B5/2Q2n2/5r2 w - - 0 100");
    std::unique_ptr<Searcher> s = std::unique_ptr<Searcher>(new Searcher(a, &out));;

    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    s->stop_searching();
    Ply p = s->find_best_ply();
    ASSERT_EQ(p, Ply("c2a4"));

    s->apply_ply(Ply("c2a4"));
    s->apply_ply(Ply("c4b5"));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    p = s->find_best_ply();
    ASSERT_EQ(p, Ply("a4d4"));

    Board b = Board("8/2k5/8/2K5/2B5/2b5/2q2N2/5R2 b - - 0 100");
    s = std::unique_ptr<Searcher>(new Searcher(b, &out));;

    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    s->stop_searching();
    p = s->find_best_ply();
    ASSERT_EQ(p, Ply("c2a4"));

    s->apply_ply(Ply("c2a4"));
    s->apply_ply(Ply("c4b5"));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    p = s->find_best_ply();
    ASSERT_EQ(p, Ply("a4d4"));
}

TEST(SearcherTest, threefold_draw) {
    std::ostringstream out = std::ostringstream();

    Board a = Board("k7/8/8/8/8/6r1/3q3P/K7 b - - 0 1");
    std::unique_ptr<Searcher> s = std::unique_ptr<Searcher>(new Searcher(a, &out));

    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    Ply p = s->find_best_ply();
    ASSERT_NE(p, Ply("b1a1"));

    s->apply_ply(Ply("a8b8"));
    s->apply_ply(Ply("a1b1"));
    s->apply_ply(Ply("b8a8"));
    s->apply_ply(Ply("b1a1"));
    s->apply_ply(Ply("a8b8"));
    s->apply_ply(Ply("a1b1"));
    s->apply_ply(Ply("b8a8"));

    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    p = s->find_best_ply();
    ASSERT_EQ(p, Ply("b1a1"));
}

/*
Not currently good enough to find this - perhaps needs NN
TEST(SearcherTest, stalemate) {
    std::ostringstream out = std::ostringstream();

    Board a = Board("2k5/4R3/1p4p1/p5Q1/qn3P2/1n6/1P6/1K6 w - - 0 100");
    std::unique_ptr<Searcher> s = std::unique_ptr<Searcher>(new Searcher(a, &out));

    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(500));
    s->stop_searching();
    Ply p = s->find_best_ply();
    ASSERT_EQ(p, Ply("g5f5"));
}
*/

TEST(SearcherTest, stopping_twice) {
    std::ostringstream out = std::ostringstream();
    std::unique_ptr<Searcher> s = std::unique_ptr<Searcher>(new Searcher(&out));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    s->stop_searching();
    Ply p = s->find_best_ply();
    ASSERT_EQ(p.from_square().get_row() < 2, true);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}