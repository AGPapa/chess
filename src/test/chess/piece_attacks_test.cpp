#include "../../chess/piece_attacks.hpp"

#include <gtest/gtest.h>

TEST(PieceAttacks, knight_attacks) {
    for (int row = 0; row <= 7; row++) {
        for (int col = 0; col <= 7; col++) {
            Square from = Square(row, col);
            for (Square to : knight_attacks[from.get_int_value()]) {
                int col_diff = abs(from.get_col() - to.get_col());
                ASSERT_EQ(true, col_diff == 1 || col_diff == 2);
                if (abs(from.get_col() - to.get_col()) == 1) {
                    ASSERT_EQ(2, abs(from.get_row() - to.get_row()));
                } else {
                    ASSERT_EQ(1, abs(from.get_row() - to.get_row()));
                }
            }
        }
    }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}