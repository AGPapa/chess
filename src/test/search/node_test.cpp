#include "../../search/root_node.cpp"
#include "../../search/leaf_node.cpp"

#include <gtest/gtest.h>

TEST(NodeTest, new_node_constructor) {
    Board b = Board::default_board();
    std::unique_ptr<RootNode> root = std::unique_ptr<RootNode>(new RootNode(b));
    ASSERT_EQ(root->is_leaf(), false);
    ASSERT_EQ(root->_child->is_leaf(), true);
}

TEST(NodeTest, convert_node_constructor) {
    Board b = Board::default_board();
    std::unique_ptr<RootNode> root = std::unique_ptr<RootNode>(new RootNode(b));

    std::unique_ptr<Node>* owner = &(root->_child);
    ASSERT_EQ(root->_child->is_leaf(), true);
    ASSERT_EQ(root->_visits, 1);

    float leaf_prior = root->_child->_prior;

    ((LeafNode *) (root->_child.get()))->convert_to_expanded_node(b, owner);
    ASSERT_EQ(root->_child->is_leaf(), false);
    ASSERT_EQ(((ExpandedNode *) (root->_child.get()))->_visits, 1);
    ASSERT_EQ(root->_child->_prior, leaf_prior);

    Node* grandchild = ((ExpandedNode *) (root->_child.get()))->_child.get();

    std::unique_ptr<RootNode> new_root = std::unique_ptr<RootNode>(new RootNode(b, (ExpandedNode *) root->_child.get()));
    ASSERT_EQ(new_root->_sibling.get(), nullptr);
    ASSERT_EQ(new_root->_child.get(), grandchild);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}