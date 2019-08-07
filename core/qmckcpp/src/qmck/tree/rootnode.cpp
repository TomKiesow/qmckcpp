#include <qmck/tree/rootnode.hpp>

#include <algorithm>

void qmck::tree::rootnode::add_child(std::unique_ptr<treenode> child)
{
    children.push_back(child.get());
    plenum.push_back(std::move(child));
}

void qmck::tree::rootnode::remove_child(treenode *child)
{
    children.erase(std::remove(children.begin(), children.end(), child), children.end());

    auto elem = std::find_if(plenum.begin(), plenum.end(), [child](std::unique_ptr<treenode> &ptr) {return ptr.get() == child;});
    if(elem != plenum.end()){
        plenum.erase(elem);
    }
}

void qmck::tree::rootnode::connect_child(qmck::tree::treenode *child)
{
    children.push_back(child);
}

void qmck::tree::rootnode::disconnect_child(qmck::tree::treenode *child)
{
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

const std::vector<qmck::tree::treenode *>& qmck::tree::rootnode::get_children() const
{
    return children;
}
