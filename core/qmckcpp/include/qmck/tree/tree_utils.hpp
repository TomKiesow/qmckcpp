#pragma once

#include <qmck/tree/tree.hpp>

#include <vector>

namespace qmck::tree::utils
{
    qmck::tree::tree build_tree(const qmck::result_table &table, int result_col);

    int calc_depth(node *node);

    node *multiply_nodes(tree &tree, node *parent_of_both, node *node1, node *node2);

    std::vector<node *> get_siblings_of(node *node);

    void remove_unneeded_braces(tree &tree);

    // probably should not be declared here
    void remove_unneeded_braces_recursion(tree &tree, node *current);
}