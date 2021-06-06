#include "../../evaluation/activation_layer.cpp"
#include "../../evaluation/mock_layer.cpp"

#include <gtest/gtest.h>

TEST(ActivationLayerTest, propagate) {
    std::int16_t input[9] = { -64, 0, 64, 128, 192, 0, 64, 128, 192 };
    MockLayer<std::int16_t> init = MockLayer<std::int16_t>(9, input);

    ActivationLayer activation = ActivationLayer(&init);

    std::int8_t output[activation.output_dimension()];
    activation.propagate(Board::default_board(), output);
    ASSERT_EQ(0, output[0]);
    ASSERT_EQ(0, output[1]);
    ASSERT_EQ(1, output[2]);
    ASSERT_EQ(2, output[3]);
    ASSERT_EQ(3, output[4]);
    ASSERT_EQ(0, output[5]);
    ASSERT_EQ(1, output[6]);
    ASSERT_EQ(2, output[7]);
    ASSERT_EQ(3, output[8]);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}