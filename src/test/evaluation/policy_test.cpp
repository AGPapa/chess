#include "../../evaluation/policy.cpp"

#include <gtest/gtest.h>

TEST(PolicyTest, add_action) {
    float value = 0.23;
    Policy policy = Policy(value);
    Ply ply = Ply("e2e4");
    float prob = 0.94;
    policy.add_action(ply, prob);

    ASSERT_EQ(policy._value, value);
    ASSERT_EQ(policy._num_actions, 1);
    ASSERT_EQ(policy._actions[0].ply().to_string(), ply.to_string());
    ASSERT_EQ(policy._actions[0].probability(), prob);
}

TEST(PolicyTest, set_value) {
    float value = 0.23;
    Policy policy = Policy(0.24);
    ASSERT_NE(policy._value, value);
    ASSERT_EQ(policy._num_actions, 0);

    policy.set_value(value);
    ASSERT_EQ(policy._value, value);
    ASSERT_EQ(policy._num_actions, 0);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}