#include "../../evaluation/action.cpp"

#include <gtest/gtest.h>

TEST(ActionTest, action_constructor) {
    Ply ply = Ply("e2e4");
    float prob = 0.94;
    Action a = Action(ply, prob);
    ASSERT_EQ(a.ply().to_string(), ply.to_string());
    ASSERT_EQ(a.probability(), prob);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}