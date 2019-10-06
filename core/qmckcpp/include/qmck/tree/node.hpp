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
        node *parent{};
        std::vector<node *> children{};
        logic_value operand{};
        bool negated{false};
        bool operation{};

    public:
        node(tree &context, node *parent, logic_value operand, bool operation);

    public:
        void remove_child(const node *);

        bool is_leaf() const;

        static bool equals(const node *n1, const node *n2);
    };
}
