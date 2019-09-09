#pragma once

#include <qmck/tree/node.hpp>
#include <qmck/result_table.hpp>

#include <memory>

namespace qmck::tree
{
    class node; // forward declaration

    class tree
    {
    public:
        // default configuration: operand = 0; operation = and
        node *rootnode;
        std::vector<std::unique_ptr<node>> all_nodes;

    public:
        tree();

        tree(tree &) = delete;

        tree(tree &&) noexcept;

        // default destructor necessary because of incomplete type in unique pointer
        ~tree() = default;

    public:
        node *create_node(qmck::logic_value operand, bool operation);

        void destroy_node(node *);
    };

    // perhaps move these functions somewhere else?
    tree build_tree(const qmck::result_table &table, int result_col);

    // removes braces around single operands
    void remove_unneeded_braces(tree &tree);

    int calc_depth(node *node);

    //    basic_node *multiply_nodes(root_node *root, basic_node *node1, basic_node *node2);
}
