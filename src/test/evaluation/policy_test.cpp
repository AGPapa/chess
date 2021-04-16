#include "../../evaluation/policy.cpp"

#include <gtest/gtest.h>

TEST(PolicyTest, add_action) {
    float value = 0.23;
    Policy policy = Policy(value);
    ASSERT_EQ(policy.value(), value);

    Ply ply = Ply("e2e4");
    float prob = 0.94;
    policy.add_action(ply, prob);
    ASSERT_EQ(policy.actions().front().ply().to_string(), ply.to_string());
    ASSERT_EQ(policy.actions().front().probability(), prob);
}

TEST(PolicyTest, set_value) {
    float value = 0.23;
    Policy policy = Policy(0.24);
    ASSERT_NE(policy.value(), value);

    policy.set_value(value);
    ASSERT_EQ(policy.value(), value);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}