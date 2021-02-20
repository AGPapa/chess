#include "../../search/searcher.cpp"

#include <gtest/gtest.h>

TEST(SearcherTest, find_best_ply) {
    Searcher s = Searcher();
    Ply p = s.find_best_ply(100);
    ASSERT_EQ(p.from_square().get_row() < 2, true);
}

TEST(SearcherTest, apply_ply) {
    Searcher s = Searcher();
    s.find_best_ply(100);
    s.apply_ply(Ply("e2e4"));
    Ply p = s.find_best_ply(100);
    ASSERT_EQ(p.from_square().get_row() > 5, true);

    s = Searcher();
    s.apply_ply(Ply("e2e4"));
    p = s.find_best_ply(100);
    ASSERT_EQ(p.from_square().get_row() > 5, true);
}

TEST(SearcherTest, initial_board_constructor) {
    Board b = Board("7k/7p/8/8/8/8/P7/K7 b - - 0 100");
    Searcher s = Searcher(b);
    Ply p = s.find_best_ply(100);
    ASSERT_EQ(p.from_square().get_row() > 5, true);
}

TEST(SearcherTest, checkmate_in_one) {
    Board a = Board("k7/pp3r2/8/8/8/8/2R3PP/7K w - - 0 100");
    Searcher s = Searcher(a);

    Ply p = s.find_best_ply(1000);
    ASSERT_EQ(p, Ply("c2c8"));

    s.apply_ply(Ply("c2c3"));
    p = s.find_best_ply(1000);
    ASSERT_EQ(p, Ply("f7f1"));

    Board b = Board("k7/1br5/8/8/7n/8/5PPP/7K w - - 0 100");
    s = Searcher(b);
    p = s.find_best_ply(1000);
    ASSERT_EQ(p, Ply("h2h3"));

    Board c = Board("7k/5ppp/8/7N/8/8/1BR5/K7 b - - 0 100");
    s = Searcher(c);
    p = s.find_best_ply(1000);
    ASSERT_EQ(p, Ply("h7h6"));

}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}