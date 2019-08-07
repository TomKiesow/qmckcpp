#pragma once

#include <qmck/tree/tree.hpp>
#include <qmck/tree/rootnode.hpp>
#include <qmck/tree/treenode.hpp>
#include <qmck/result_table.hpp>

namespace qmck::tree
{
    rootnode build_tree(const qmck::result_table &table, int result_col);

    // returns true when an element of parent.children was removed, otherwise false
    bool remove_unneeded_braces_recursive(rootnode &parent, treenode *current);

    // returns true when an element of parent.children was removed, otherwise false
    bool remove_unneeded_braces_recursive(treenode *parent, treenode *current);

    // removes braces around single operands
    void remove_unneeded_braces(rootnode &rootnode);
}
