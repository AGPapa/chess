#include "leaf_node.hpp"
#include "expander.cpp"

float LeafNode::convert_to_expanded_node(Board b, std::unique_ptr<Node> *owner) {
    std::unique_ptr<Node> new_node = std::unique_ptr<Node>(new ExpandedNode(std::move(_sibling), nullptr, _ply, 0.0, _prior));
    float value = Expander::evaluate_and_expand(b, (ExpandedNode*) new_node.get());
    (*owner) = std::move(new_node);
    return value;
}
