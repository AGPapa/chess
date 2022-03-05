#include "../../search/root_node.cpp"
#include "../../search/expander.cpp"

#include <gtest/gtest.h>

TEST(NodeTest, new_node_constructor) {
    Board b = Board::default_board();
    std::unique_ptr<RootNode> root = std::unique_ptr<RootNode>(new RootNode(b));
    for (int i = 0; i < root->_num_children; i++) {
      ASSERT_EQ(root->_children[i].is_leaf(), true);
    }
}

TEST(NodeTest, convert_node_constructor) {
    Board b = Board::default_board();
    std::unique_ptr<RootNode> root = std::unique_ptr<RootNode>(new RootNode(b));

    ASSERT_EQ(root->_visits, 1);

    Edge* child = &root->_children[0];
    ASSERT_EQ(child->is_leaf(), true);

    float leaf_prior = child->_prior;

    Expander::expand(b.is_white_turn(), Evaluator::evaluate(b).get(), child);
    ASSERT_EQ(child->is_leaf(), false);
    ASSERT_EQ(child->_node->_visits, 1);
    ASSERT_EQ(child->_prior, leaf_prior);
    ASSERT_EQ(child->search_score(root->_visits), child->_node->search_score(root->_visits, child->_prior));

    Edge* grandchild = &child->_node->_children[0];

    std::unique_ptr<RootNode> new_root = std::unique_ptr<RootNode>(new RootNode(b, child->_ply, child->_node.get()));
    ASSERT_EQ(&new_root->_children[0], grandchild);
    ASSERT_EQ(new_root->_visits, 1);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}