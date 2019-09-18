#include <qmck/tree/node.hpp>

#include <algorithm>

qmck::tree::node::node(tree &context, node *parent, logic_value operand, bool operation) : context(context), parent(parent), operand(operand), operation(operation)
{
}

void qmck::tree::node::remove_child(const node *child)
{
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

bool qmck::tree::node::is_leaf() const
{
    return children.empty();
}
