#include "../../search/root_node.cpp"
#include "../../search/expand_job.cpp"
#include "../../search/edge.cpp"
#include "../../evaluation/evaluator.cpp"

#include <gtest/gtest.h>

TEST(ExpandJobTest, run) {
    Board b = Board::default_board();
    std::unique_ptr<RootNode> root = std::unique_ptr<RootNode>(new RootNode(b));
    Edge* leaf = &(root->_children[0]);
    std::unique_ptr<Policy> p = Evaluator::evaluate(b, leaf->_ply);

    std::unique_ptr<std::vector<Node*>> lineage = std::unique_ptr<std::vector<Node*>>(new std::vector<Node*>());
    lineage->push_back(root.get());

    ExpandJob j = ExpandJob(b.is_white_turn(), std::move(p), leaf, std::move(lineage));
    std::set<Edge*> active_nodes = std::set<Edge*>();
    active_nodes.insert(leaf);
    std::unique_ptr<MPSCQueue<BackpropJob>> backprop_queue = std::unique_ptr<MPSCQueue<BackpropJob>>(new MPSCQueue<BackpropJob>());

    ASSERT_EQ(leaf->_node, nullptr);
    ASSERT_EQ(active_nodes.count(leaf), 1);
    ASSERT_EQ(backprop_queue->size(), 0);

    j.run(&active_nodes, backprop_queue.get());

    ASSERT_NE(leaf->_node, nullptr);
    ASSERT_EQ(active_nodes.count(leaf), 0);
    ASSERT_EQ(backprop_queue->size(), 1);
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