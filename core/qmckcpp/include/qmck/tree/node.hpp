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
        tree &parent;
        std::vector<node *> children;
        qmck::logic_value operand;
        bool negated = false;
        bool operation;

    public:
        node(tree &parent, qmck::logic_value operand, bool operation);

    public:
        void remove_child(node *);
    };
}
