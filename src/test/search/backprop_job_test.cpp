#include "../../search/root_node.cpp"
#include "../../search/backprop_job.cpp"
#include "../../search/leaf_node.cpp"

#include <gtest/gtest.h>

TEST(BackpropJobTest, run) {
    Board b = Board("6k1/7R/8/8/8/7R/R6Q/R6K w - - 0 100");
    std::unique_ptr<RootNode> root = std::unique_ptr<RootNode>(new RootNode(b));
    std::unique_ptr<std::vector<ExpandedNode*>> lineage = std::unique_ptr<std::vector<ExpandedNode*>>(new std::vector<ExpandedNode*>());
    lineage->push_back(root.get());

    b.apply_ply(root->_child->_ply);
    ((LeafNode*) root->_child.get())->convert_to_expanded_node(b, &(root->_child));
    ExpandedNode* expanded_child = (ExpandedNode*) root->_child.get();
    lineage->push_back(expanded_child);
    
    int prior_visits = root->_visits;
    float prior_score = root->_score;
    float score_increment = 0.2;

    BackpropJob(score_increment, std::move(lineage), b.is_white_turn()).run();

    ASSERT_EQ(root->_visits, prior_visits + 1);
    ASSERT_EQ(root->_score, prior_score - score_increment);

    lineage = std::unique_ptr<std::vector<ExpandedNode*>>(new std::vector<ExpandedNode*>());
    lineage->push_back(root.get());
    lineage->push_back(expanded_child);

    b.apply_ply(expanded_child->_child->_ply);
    ((LeafNode*) expanded_child->_child.get())->convert_to_expanded_node(b, &(expanded_child->_child));
    ExpandedNode* expanded_grandchild = (ExpandedNode*) expanded_child->_child.get();
    lineage->push_back(expanded_grandchild);

    prior_visits = root->_visits;
    prior_score = root->_score;
    int prior_child_visits = expanded_child->_visits;
    float prior_child_score = expanded_child->_score;
    BackpropJob(score_increment, std::move(lineage), b.is_white_turn()).run();

    ASSERT_EQ(root->_visits, prior_visits + 1);
    ASSERT_EQ(root->_score, prior_score - score_increment);
    ASSERT_EQ(expanded_child->_visits, prior_child_visits + 1);
    ASSERT_EQ(expanded_child->_score, prior_child_score + score_increment);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}