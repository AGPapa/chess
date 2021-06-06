#include "../../evaluation/transformation_layer.cpp"

#include <gtest/gtest.h>

// TODO: test correct mirroring
TEST(TransformationLayerTest, propagate) {
    TransformationLayer layer = TransformationLayer();

    std::int8_t output[layer.output_dimension()] = {};
    layer.propagate(Board::default_board(), output);
    ASSERT_EQ(0, output[0]);
    ASSERT_EQ(0, output[PIECE_RELATIONS*4]);
    int overall_count = 0;

    // friendly pov
    for (int i = 0; i < layer.output_dimension(); i++) {
        ASSERT_EQ(true, output[i] == 0 || output[i] == 1);
        if (output[i] == 1) { overall_count++; }
    }
    ASSERT_EQ(overall_count, 68);

    int friendly_perspective = 0;
    for (int i = 2560; i < 3200; i++) {
        ASSERT_EQ(true, output[i] == 0 || output[i] == 1);
        if (output[i] == 1) { friendly_perspective++; }
    }
    ASSERT_EQ(friendly_perspective, 30);

    // enemy pov
    int enemy_perspective = 0;
    for (int i = 79364; i < 80004; i++) {
        ASSERT_EQ(true, output[i] == 0 || output[i] == 1);
        if (output[i] == 1) { enemy_perspective++; }
    }
    ASSERT_EQ(enemy_perspective, 30);

    // castling
    ASSERT_EQ(output[40960], 1);
    ASSERT_EQ(output[40961], 1);
    ASSERT_EQ(output[40962], 1);
    ASSERT_EQ(output[40963], 1);

    ASSERT_EQ(output[81924], 1);
    ASSERT_EQ(output[81925], 1);
    ASSERT_EQ(output[81926], 1);
    ASSERT_EQ(output[81927], 1);
}

TEST(TransformationLayerTest, output_dimension) {
    TransformationLayer layer = TransformationLayer();

    ASSERT_EQ(81928, layer.output_dimension());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}