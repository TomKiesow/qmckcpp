#include <qmck/tree/tree.hpp>

#include <algorithm>

qmck::tree::rootnode qmck::tree::build_tree(const qmck::result_table &result, int result_col)
{
    tree::rootnode rootnode{};
    for (std::size_t i{0}; i < (1u << result.format.inputs_count); i++)
    {
        auto child = std::make_unique<treenode>(rootnode, 0);
        rootnode.add_child(std::move(child)); // rootnode manages memory!
    }

    for (auto &pair : result.rows)
    {
        auto &row = pair.second;
        auto id = pair.first;

        if ((row.outputs >> (result.format.outputs_count - result_col - 1)) & 1u)
        {
            auto minterms_covered = row.calculate_minterms_covered();

            for (auto minterm : minterms_covered)
            {
                treenode *minterm_node_ptr = rootnode.get_children()[minterm];
                treenode &minterm_node = (*minterm_node_ptr);

                auto new_node = std::make_unique<treenode>(rootnode, id);
                minterm_node.add_child(std::move(new_node));
            }
        }
    }

    auto &roots_children = rootnode.get_children();
    for (std::size_t child_i{0}; child_i < roots_children.size(); ++child_i)
    {
        auto &child = roots_children[child_i];

        if (child->get_children().empty())
        {
            rootnode.remove_child(child);
            --child_i;
        }
    }

    return rootnode;
}

bool qmck::tree::remove_unneeded_braces_recursive(rootnode &parent, treenode *current)
{
    bool return_value{false};

    auto &children = current->get_children();
    if (children.size() == 1)
    {
        auto &child = children[0];
        current->disconnect_child(child);
        parent.connect_child(child);
        parent.remove_child(current);

        return_value = true;
    }
    else if (children.size() > 1)
    {
        for (std::size_t child_i{0}; child_i < children.size(); ++child_i)
        {
            auto &child = children[child_i];

            // if a child was removed from current.children, --child_i compensates
            if(remove_unneeded_braces_recursive(current, child)){
                --child_i;
            }
        }
    }

    return return_value;
}

bool qmck::tree::remove_unneeded_braces_recursive(treenode *parent, treenode *current)
{
    bool return_value{false};

    auto &children = current->get_children();
    if (children.size() == 1)
    {
        auto &child = children[0];
        current->disconnect_child(child);
        parent->connect_child(child);
        parent->remove_child(current);

        return_value = true;
    }
    else if (children.size() > 1)
    {
        for (std::size_t child_i{0}; child_i < children.size(); ++child_i)
        {
            auto &child = children[child_i];

            // if a child was removed from current.children, --child_i compensates
            if(remove_unneeded_braces_recursive(current, child)){
                --child_i;
            }
        }
    }

    return return_value;
}

void qmck::tree::remove_unneeded_braces(qmck::tree::rootnode &rootnode)
{
    auto &children = rootnode.children;

    for (std::size_t child_i{0}; child_i < children.size(); ++child_i)
    {
        auto &child = children[child_i];

        // if a child was removed from rootnode.children, --child_i compensates
        if(remove_unneeded_braces_recursive(rootnode, child)){
            --child_i;
        }
    }
}
