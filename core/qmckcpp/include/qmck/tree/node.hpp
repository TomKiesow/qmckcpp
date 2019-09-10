#pragma once

#include <qmck/tree/tree.hpp>
#include <qmck/logic_value.hpp>

#include <vector>

namespace qmck::tree
{
    class tree; // forward declaration

    const bool OPERATION_AND = true;
    const bool OPERATION_OR = false;

    class node
    {

    public:
        tree &context;
        node *parent;
        std::vector<node *> children;
        qmck::logic_value operand;
        bool negated = false;
        bool operation;

    public:
        node(tree &context, node *parent, qmck::logic_value operand, bool operation);

    public:
        void remove_child(node *);

        bool is_leaf() const;
    };
}
