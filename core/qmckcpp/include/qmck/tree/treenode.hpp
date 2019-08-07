#pragma once

#include <qmck/logic_value.hpp>
#include <qmck/tree/rootnode.hpp>

#include <memory>
#include <vector>
#include <qmck/quine_row.hpp>

namespace qmck::tree
{
    struct rootnode; // forward declaration

    struct treenode
    {
    public:
        qmck::tree::rootnode &root;
        qmck::logic_value operand;
        std::vector<treenode *> children{};

    public:
        treenode(rootnode &rootnode, qmck::logic_value minterm);

        void add_child(std::unique_ptr<qmck::tree::treenode> child);

        void remove_child(treenode *child);

        void connect_child(qmck::tree::treenode *child);

        void disconnect_child(qmck::tree::treenode *child);

        const std::vector<treenode *>& get_children() const;
    };
}
