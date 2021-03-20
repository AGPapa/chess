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
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    Ply p = s->find_best_ply();
    ASSERT_EQ(p, Ply("c2c8"));

    s->apply_ply(Ply("c2c3"));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    p = s->find_best_ply();
    ASSERT_EQ(p, Ply("f7f1"));

    Board b = Board("k7/1br5/8/8/7n/8/5PPP/7K w - - 0 100");
    s = std::unique_ptr<Searcher>(new Searcher(b, &out));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    p = s->find_best_ply();
    ASSERT_EQ(p, Ply("h2h3"));

    Board c = Board("7k/5ppp/8/7N/8/8/1BR5/K7 b - - 0 100");
    s = std::unique_ptr<Searcher>(new Searcher(c, &out));
    s->start_searching();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    s->stop_searching();
    p = s->find_best_ply();
    ASSERT_EQ(p, Ply("h7h6"));
}

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