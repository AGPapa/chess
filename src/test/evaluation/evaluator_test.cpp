#include "../../evaluation/evaluator.cpp"

#include <gtest/gtest.h>

TEST(EvaluatorTest, evaluate_threefold_repetition) {
    Board b = Board("8/6pk/8/8/8/8/8/K6Q b - - 0 100");
    b.apply_ply(Ply("h7g8"));
    b.apply_ply(Ply("h1a8"));
    std::unique_ptr<Policy> p = std::move(Evaluator::evaluate(b));
    ASSERT_NE(0, p->value());
    ASSERT_NE(0, p->actions()->size());

    b.apply_ply(Ply("g8h7"));
    b.apply_ply(Ply("a8h1"));
    p = std::move(Evaluator::evaluate(b));
    ASSERT_NE(0, p->value());
    ASSERT_NE(0, p->actions()->size());

    b.apply_ply(Ply("h7g8"));
    b.apply_ply(Ply("h1a8"));
    p = std::move(Evaluator::evaluate(b));
    ASSERT_NE(0, p->value());
    ASSERT_NE(0, p->actions()->size());

    b.apply_ply(Ply("g8h7"));
    b.apply_ply(Ply("a8h1"));
    p = std::move(Evaluator::evaluate(b));
    ASSERT_EQ(0, p->value());
    ASSERT_EQ(0, p->actions()->size());
}

TEST(EvaluatorTest, evaluate_checkmate) {
    Board b = Board("R6k/6pp/8/8/8/8/8/7K b - - 0 100");
    std::unique_ptr<Policy> p = std::move(Evaluator::evaluate(b));

    ASSERT_EQ(1, p->value());
    ASSERT_EQ(0, p->actions()->size());


    b = Board("7k/8/8/8/8/8/6PP/r6K w - - 0 100");
    p = std::move(Evaluator::evaluate(b));

    ASSERT_EQ(-1, p->value());
    ASSERT_EQ(0, p->actions()->size());
}

TEST(EvaluatorTest, evaluate_stalemate) {
    Board b = Board("7k/8/6Q1/8/8/8/8/7K b - - 0 100");
    std::unique_ptr<Policy> p = std::move(Evaluator::evaluate(b));

    ASSERT_EQ(0, p->value());
    ASSERT_EQ(0, p->actions()->size());


    b = Board("7k/8/8/8/8/6q1/8/7K w - - 0 100");
    p = std::move(Evaluator::evaluate(b));

    ASSERT_EQ(0, p->value());
    ASSERT_EQ(0, p->actions()->size());
}

TEST(EvaluatorTest, evaluate_fifty_move_draw) {
  Board b = Board("7k/8/8/8/8/6r1/8/7K w - - 99 100");
  std::unique_ptr<Policy> p = std::move(Evaluator::evaluate(b));

  ASSERT_NE(0, p->value());
  ASSERT_NE(0, p->actions()->size());

  b.apply_ply(Ply("h1h2"));
    p = std::move(Evaluator::evaluate(b));

  ASSERT_EQ(0, p->value());
  ASSERT_EQ(0, p->actions()->size());
}

TEST(EvaluatorTest, is_insufficient_mating_material) {
  Board b = Board("7k/8/8/8/8/8/8/6RK w - - 1 100");
  std::unique_ptr<Policy> p = std::move(Evaluator::evaluate(b));

  ASSERT_NE(0, p->value());
  ASSERT_NE(0, p->actions()->size());

  b.apply_ply(Ply("g1g8"));
  b.apply_ply(Ply("h8g8"));
  p = std::move(Evaluator::evaluate(b));

  ASSERT_EQ(0, p->value());
  ASSERT_EQ(0, p->actions()->size());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}