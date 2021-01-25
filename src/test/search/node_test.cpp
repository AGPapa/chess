#include "../../search/root_node.cpp"

#include <gtest/gtest.h>

TEST(NodeTest, new_node_constructor) {
    Board b = Board::default_board();
    RootNode root = RootNode(b);
    ASSERT_EQ(root.is_leaf(), false);
    ASSERT_EQ(root._child->is_leaf(), true);
}

TEST(NodeTest, convert_node_constructor) {
    Board b = Board::default_board();
    RootNode root = RootNode(b);
    std::unique_ptr<Node>* owner = &(root._child);
    ASSERT_EQ(root._child->is_leaf(), true);

    ((LeafNode *) (root._child.get()))->expand(b, owner);
    ASSERT_EQ(root._child->is_leaf(), false);
    ASSERT_EQ(((ExpandedNode *) (root._child.get()))->_visits, 1);

    Node* grandchild = ((ExpandedNode *) (root._child.get()))->_child.get();

    RootNode new_root = RootNode(b, (ExpandedNode *) root._child.get());
    ASSERT_EQ(new_root._sibling.get(), nullptr);
    ASSERT_EQ(new_root._parent, nullptr);
    ASSERT_EQ(new_root._child.get(), grandchild);

    for (Node *child : new_root.children()) {
      ASSERT_EQ(child->_parent, &new_root);
    }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}