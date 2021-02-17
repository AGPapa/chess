#include "../../search/searcher.cpp"

#include <gtest/gtest.h>

TEST(SearcherTest, find_best_ply) {
    Searcher s = Searcher();
    Ply p = s.find_best_ply(1000);
    ASSERT_EQ(p.from_square().get_row() < 2, true);
}

TEST(SearcherTest, apply_ply) {
    Searcher s = Searcher();
    s.find_best_ply(1000);
    s.apply_ply(Ply("e2e4"));
    Ply p = s.find_best_ply(1000);
    ASSERT_EQ(p.from_square().get_row() > 5, true);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}