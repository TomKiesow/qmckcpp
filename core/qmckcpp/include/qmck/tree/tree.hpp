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
        node *rootnode{};
        std::vector<std::unique_ptr<node>> all_nodes{};

    public:
        tree();

        tree(tree &) = delete;

        tree(tree &&) noexcept;

        // default destructor necessary because of incomplete type in unique pointer
        ~tree() = default;

    public:
        node *create_node(node *parent, logic_value operand, bool operation);

        void destroy_node(node *);

        void destroy_subtree(node *);
    };
}
