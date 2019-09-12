#pragma once

#include <qmck/tree/tree.hpp>

#include <vector>

namespace qmck::tree::utils
{
    qmck::tree::tree build_tree(const qmck::result_table &table, int result_col);

    int calc_depth(node *node);

    std::vector<node *> get_siblings_of(node *node);

    void remove_unneeded_braces(tree &tree);

    // probably should be declared elsewhere
    void remove_unneeded_braces_recursion(tree &tree, node *current);

    void simplify_idempotency(tree &tree);

    node *duplicate_subtree(tree &tree, node *subtree);

    node *multiply_nodes(tree &tree, qmck::tree::node *parent, node *node1, node *node2);
}