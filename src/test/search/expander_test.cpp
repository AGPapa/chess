#include "../../search/root_node.cpp"
#include "../../search/expander.cpp"
#include "../../search/leaf_node.cpp"

#include <gtest/gtest.h>

TEST(ExpanderTest, backpropagate) {
    Board b = Board("6k1/7R/8/8/8/7R/R6Q/R6K w - - 0 100");
    RootNode root = RootNode(b);
    b.apply_ply(root._child->_ply);
    ((LeafNode*) root._child.get())->convert_to_expanded_node(b, &(root._child));
    ExpandedNode* expanded_child = (ExpandedNode*) root._child.get();
    
    int prior_visits = root._visits;
    float prior_score = root._score;
    float score_increment = 0.2;

    Expander::backpropagate(score_increment, expanded_child, b.is_white_turn());

    ASSERT_EQ(root._visits, prior_visits + 1);
    ASSERT_EQ(root._score, prior_score + score_increment);

    b.apply_ply(expanded_child->_child->_ply);
    ((LeafNode*) expanded_child->_child.get())->convert_to_expanded_node(b, &(expanded_child->_child));
    ExpandedNode* expanded_grandchild = (ExpandedNode*) expanded_child->_child.get();

    prior_visits = root._visits;
    prior_score = root._score;
    Expander::backpropagate(score_increment, expanded_child, b.is_white_turn());

    ASSERT_EQ(root._visits, prior_visits + 1);
    ASSERT_EQ(root._score, prior_score - score_increment);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}