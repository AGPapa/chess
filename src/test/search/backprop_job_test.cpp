#include "../../search/root_node.cpp"
#include "../../search/backprop_job.cpp"
#include "../../search/edge.cpp"
#include "../../search/expander.cpp"

#include <gtest/gtest.h>

TEST(BackpropJobTest, run) {
    Board b = Board("6k1/7R/8/8/8/7R/R6Q/R6K w - - 0 100");
    std::unique_ptr<RootNode> root = std::unique_ptr<RootNode>(new RootNode(b));
    std::unique_ptr<std::vector<Node*>> lineage = std::unique_ptr<std::vector<Node*>>(new std::vector<Node*>());
    lineage->push_back(root.get());

    b.apply_ply(root->_children[0]._ply);
    Expander::expand(b.is_white_turn(), Evaluator::evaluate(b).get(), &root->_children[0]);
    Node* expanded_child = root->_children[0]._node.get();
    lineage->push_back(expanded_child);
    
    float prior_score = root->_score;
    float score_increment = 0.2;

    BackpropJob(score_increment, std::move(lineage), b.is_white_turn()).run();

    ASSERT_EQ(root->_score, prior_score - score_increment + 1);

    lineage = std::unique_ptr<std::vector<Node*>>(new std::vector<Node*>());
    lineage->push_back(root.get());
    lineage->push_back(expanded_child);

    b.apply_ply(expanded_child->_children[0]._ply);
    Expander::expand(b.is_white_turn(), Evaluator::evaluate(b).get(), &expanded_child->_children[0]);
    Node* expanded_grandchild = expanded_child->_children[0]._node.get();
    lineage->push_back(expanded_grandchild);

    prior_score = root->_score;
    float prior_child_score = expanded_child->_score;
    BackpropJob(score_increment, std::move(lineage), b.is_white_turn()).run();

    ASSERT_EQ(root->_score, prior_score - score_increment + 1);
    ASSERT_EQ(expanded_child->_score, prior_child_score + score_increment + 1);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}