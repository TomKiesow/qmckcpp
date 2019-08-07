#pragma once

#include <qmck/logic_value.hpp>
#include <qmck/tree/treenode.hpp>

#include <memory>
#include <vector>
#include <qmck/quine_row.hpp>

namespace qmck::tree
{
    struct treenode; // forward declaration

    struct rootnode
    {
    public:
        std::vector<treenode *> children{};
        std::vector<std::unique_ptr<treenode>> plenum{};
    public:
        rootnode() = default;

        void add_child(std::unique_ptr<qmck::tree::treenode> child);

        void remove_child(treenode *child);

        void connect_child(qmck::tree::treenode *child);

        void disconnect_child(qmck::tree::treenode *child);

        const std::vector<treenode *>& get_children() const;
    };
}
