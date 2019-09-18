#pragma once

#include <qmck/tree/tree.hpp>

#include <vector>

namespace qmck::tree::utils
{
    qmck::tree::tree build_tree(const qmck::result_table &table, int result_col);

    bool node_comparator(const node* a, const node* b);

    int calc_depth(const node *node);

    void remove_unneeded_braces(tree &tree);

    // probably should be declared elsewhere
    void remove_unneeded_braces_recursion(tree &tree, node *current);

    void simplify_idempotency(tree &tree);

    void simplify_absorption(tree &tree);

    // declare somewhere else, too?
    bool simplify_absorption(tree &tree, node* current);

    node *duplicate_subtree(tree &tree, node *subtree);

    node *multiply_nodes(tree &tree, node *parent, node *node1, node *node2);

    bool has_petrick_result_form(const tree &tree);

    std::vector<qmck::logic_value> get_all_operands(const node* node);

    void get_all_operands_recursion(const node* current, std::vector<qmck::logic_value> &values);
}