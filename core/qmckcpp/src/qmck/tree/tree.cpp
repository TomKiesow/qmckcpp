#include <qmck/tree/tree.hpp>

#include <algorithm>

qmck::tree::tree::tree()
{
    this->rootnode = create_node(nullptr, 0, OPERATION_AND);
}

qmck::tree::tree::tree(tree &&other) noexcept
{
    this->rootnode = other.rootnode;
    other.rootnode = other.create_node(nullptr, 0, OPERATION_AND); // tree without rootnode is invalid
    this->all_nodes = std::move(other.all_nodes);
}

qmck::tree::node *qmck::tree::tree::create_node(node *parent, logic_value operand, bool operation)
{
    std::unique_ptr<node> new_node = std::make_unique<node>(*this, parent, operand, operation);
    node *new_node_ptr = new_node.get();
    all_nodes.push_back(std::move(new_node));

    return new_node_ptr;
}

void qmck::tree::tree::destroy_node(qmck::tree::node *target)
{
    all_nodes.erase(std::remove_if(all_nodes.begin(), all_nodes.end(), [target](std::unique_ptr<node> &node)
    {
        return node.get() == target;
    }), all_nodes.end());
}

void qmck::tree::tree::destroy_subtree(qmck::tree::node *subtree)
{
    for(auto child:subtree->children){
        destroy_subtree(child);
    }
    destroy_node(subtree);
}
