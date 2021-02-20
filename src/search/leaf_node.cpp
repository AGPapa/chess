#include "leaf_node.hpp"
#include "expander.cpp"

void LeafNode::convert_to_expanded_node(Board b, std::unique_ptr<Node> *owner, std::vector<ExpandedNode*> lineage) {
    std::unique_ptr<Node> new_node = std::unique_ptr<Node>(new ExpandedNode(std::move(_sibling), nullptr, _ply, 0.0, _probability));
    Expander::evaluate_and_expand(b, (ExpandedNode*) new_node.get(), lineage);
    (*owner) = std::move(new_node);
}
