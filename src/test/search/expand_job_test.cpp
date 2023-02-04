#include "../../search/root_node.cpp"
#include "../../search/expand_job.cpp"
#include "../../search/edge.cpp"
#include "../../evaluation/evaluator.cpp"

#include <gtest/gtest.h>

//TODO: Add backprop tests into this file
TEST(ExpandJobTest, run) {
    Board b = Board::default_board();
    std::unique_ptr<RootNode> root = std::unique_ptr<RootNode>(new RootNode(b, BoardHistory()));
    Edge* leaf = &(root->_children[0]);
    std::unique_ptr<Policy> p = Evaluator::evaluate(b);

    float prior_score = root->_score;
    float score_increment = 0.2;
    p->set_value(score_increment);

    Bank<std::vector<Node*>> lineage_bank = Bank<std::vector<Node*>>();
    std::pair<std::vector<Node*>*, int> lineage_bank_pair = lineage_bank.acquire();
    std::vector<Node*>* lineage = lineage_bank_pair.first;
    lineage->push_back(root.get());

    ExpandJob j = ExpandJob(b.is_white_turn(), std::move(p), leaf, lineage_bank_pair.second);
    std::set<Edge*> active_nodes = std::set<Edge*>();
    active_nodes.insert(leaf);

    ASSERT_EQ(leaf->_node, nullptr);
    ASSERT_EQ(active_nodes.count(leaf), 1);

    j.run(&active_nodes, &lineage_bank);

    ASSERT_NE(leaf->_node, nullptr);
    ASSERT_EQ(active_nodes.count(leaf), 0);
    ASSERT_EQ(root->_score, prior_score - score_increment + 1);
    int i = 0;
    for (int i = 0; i < leaf->_node->_num_children; i++) {
      ASSERT_EQ(leaf->_node->_children[i]._node, nullptr);
      ASSERT_NE(leaf->_node->_children[i]._ply, Ply());
      ASSERT_NE(leaf->_node->_children[i]._prior, 0);
    }


}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}