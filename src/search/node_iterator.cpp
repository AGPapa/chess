#include "node.cpp"

class NodeIterator {

    public:
        NodeIterator(Node **node) {
            if (node == nullptr) {
                _node = nullptr;
            } else {
                _node = *node;
            }
        }

        NodeIterator begin() {
            return NodeIterator(&_node);
        }

        NodeIterator end() {
            return NodeIterator(nullptr);
        }

        NodeIterator& operator++() {
            if (_node != nullptr)
                _node = _node->_sibling.get();
            return *this;
        }

        NodeIterator operator++(int) {
            NodeIterator iterator = *this;
            ++*this;
            return iterator;
        }

        bool operator!=(const NodeIterator& iterator) {
            return _node != iterator._node;
        }

        Node* operator*() {
            return _node;
        }

    private:
        Node *_node;

};
