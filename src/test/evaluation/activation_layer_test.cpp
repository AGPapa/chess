#include "../../evaluation/activation_layer.cpp"
#include "../../evaluation/initial_layer.cpp"

#include <gtest/gtest.h>

TEST(ActivationLayerTest, propagate) {
    std::int16_t input[4] = { -1, 0, 1, 2 };
    InitialLayer init = InitialLayer(4, input);

    ActivationLayer activation = ActivationLayer(&init);

    std::int16_t output[activation.output_dimension()];
    activation.propagate(output);
    ASSERT_EQ(0, output[0]);
    ASSERT_EQ(0, output[1]);
    ASSERT_EQ(1, output[2]);
    ASSERT_EQ(2, output[3]);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}