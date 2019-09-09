#include <qmck/tree/node.hpp>

#include <algorithm>

qmck::tree::node::node(qmck::tree::tree &parent, qmck::logic_value operand, bool operation) : parent(parent), operand(operand), operation(operation)
{
}

void qmck::tree::node::remove_child(qmck::tree::node *child)
{
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}
