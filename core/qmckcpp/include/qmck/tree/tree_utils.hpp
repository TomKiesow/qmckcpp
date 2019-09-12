#pragma once

#include <qmck/tree/tree.hpp>

#include <vector>

namespace qmck::tree::utils
{
    qmck::tree::tree build_tree(const qmck::result_table &table, int result_col);

    bool node_comparator(const node* a, const node* b);

    int calc_depth(node *node);

    void remove_unneeded_braces(tree &tree);

    // probably should be declared elsewhere
    void remove_unneeded_braces_recursion(tree &tree, node *current);

    void simplify_idempotency(tree &tree);

    void simplify_absorption(tree &tree);

    bool simplify_absorption(tree &tree, node* current);

    node *duplicate_subtree(tree &tree, node *subtree);

    node *multiply_nodes(tree &tree, qmck::tree::node *parent, node *node1, node *node2);
}