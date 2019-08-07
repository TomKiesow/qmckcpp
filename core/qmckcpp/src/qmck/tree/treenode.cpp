#include <qmck/tree/treenode.hpp>

#include <algorithm>

qmck::tree::treenode::treenode(rootnode &root, qmck::logic_value operand) : root(root), operand(operand)
{
}

void qmck::tree::treenode::add_child(std::unique_ptr<qmck::tree::treenode> child)
{
    children.push_back(child.get());
    root.plenum.push_back(std::move(child));
}

void qmck::tree::treenode::remove_child(treenode *child)
{
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
    auto elem = std::find_if(root.plenum.begin(), root.plenum.end(), [child](std::unique_ptr<treenode> &ptr)
    {
        return ptr.get() == child;
    });
    if (elem != root.plenum.end())
    {
        root.plenum.erase(elem);
    }
}

void qmck::tree::treenode::connect_child(qmck::tree::treenode *child)
{
    children.push_back(child);
}

void qmck::tree::treenode::disconnect_child(qmck::tree::treenode *child)
{
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

const std::vector<qmck::tree::treenode *> &qmck::tree::treenode::get_children() const
{
    return children;
}
