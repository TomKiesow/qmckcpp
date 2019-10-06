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

bool qmck::tree::node::equals(const node *n1, const node *n2)
{
    auto children1 = n1->children;
    auto children2 = n2->children;

    std::sort(children1.begin(), children1.end());
    std::sort(children2.begin(), children2.end());

    if (n1->is_leaf() && n2->is_leaf())
    {
        if(n1->operand == n2->operand && n1->negated == n2->negated)
        return true;
    }
    else if (!n1->is_leaf() && !n2->is_leaf())
    {
        if (children1.size() == children2.size())
        {
            for (std::size_t i{0}; i < children1.size(); ++i)
            {
                if(!equals(children1[i], children2[i])){
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}
