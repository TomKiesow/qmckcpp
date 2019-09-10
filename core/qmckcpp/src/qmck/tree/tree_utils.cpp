#include <qmck/tree/tree_utils.hpp>

#include <algorithm>

qmck::tree::tree qmck::tree::utils::build_tree(const qmck::result_table &result, int result_col)
{
    tree tree{};
    auto rootnode = tree.rootnode;
    for (std::size_t i{0}; i < (1u << result.format.inputs_count); i++)
    {
        auto new_node = tree.create_node(rootnode, 0, OPERATION_OR);
        rootnode->children.push_back(new_node);
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
                auto minterm_node_ptr = rootnode->children[minterm];

                auto new_node = tree.create_node(minterm_node_ptr, id, OPERATION_AND); // OPERATION_AND irrelevant because node is leaf!
                minterm_node_ptr->children.push_back(new_node);
            }
        }
    }

    auto &roots_children = rootnode->children;
    for (std::size_t child_i{0}; child_i < roots_children.size(); ++child_i)
    {
        auto child = roots_children[child_i];

        if (child->children.empty())
        {
            rootnode->remove_child(child);
            tree.destroy_node(child);
            --child_i;
        }
    }

    return tree;
}

int qmck::tree::utils::calc_depth(node *node)
{
    std::vector<int> children_depths{};

    for (auto &child : node->children)
    {
        if (child->children.empty())
        {
            children_depths.push_back(1);
        }
        else
        {
            children_depths.push_back(1 + calc_depth(child));
        }
    }

    int max = 0;
    for (auto &depth : children_depths)
    {
        if (depth > max)
        {
            max = depth;
        }
    }

    return max;
}

qmck::tree::node *qmck::tree::utils::multiply_nodes(tree &tree, node *parent, node *child1, node *child2)
{
    auto result_parent_ptr = tree.create_node(nullptr, 0, parent->operation);

    //    for (auto &n1 : child1->children)
    //    {
    //        for (auto &n2 : child2->children)
    //        {
    //            auto result_parent1_unique_ptr = std::make_unique<internal_node>(*root, 0);
    //            auto result_parent_ptr1 = result_parent1_unique_ptr.get();
    //
    //            result_parent_ptr->connect_child(result_parent_ptr1);
    //            result_parent_ptr->add_node(std::move(result_parent1_unique_ptr));
    //
    //            auto result1 = std::make_unique<internal_node>(*root, 0);
    //            auto result2 = std::make_unique<internal_node>(*root, 0);
    //
    //            result1->operand = n1->operand;
    //            result2->operand = n2->operand;
    //
    //            result_parent_ptr1->connect_child(result1.get());
    //            result_parent_ptr1->connect_child(result2.get());
    //
    //            root->add_node(std::move(result1));
    //            root->add_node(std::move(result2));
    //        }
    //    }

    return result_parent_ptr;
}

std::vector<qmck::tree::node *> get_siblings_of(qmck::tree::node *node)
{
    std::vector<qmck::tree::node *> all_children = node->parent->children;
    all_children.erase(std::remove(all_children.begin(), all_children.end(), node), all_children.end());

    return all_children;
}

void qmck::tree::utils::remove_unneeded_braces(qmck::tree::tree &tree)
{
    remove_unneeded_braces_recursion(tree, tree.rootnode);
}

void qmck::tree::utils::remove_unneeded_braces_recursion(qmck::tree::tree &tree, qmck::tree::node *current)
{
    auto &children = current->children;

    // if current has only one child
    // remove child and connect its children to self
    if (children.size() == 1)
    {
        auto &child = children[0];
        for (auto childs_child : child->children)
        {
            childs_child->parent = current;
        }
        current->operand = child->operand;
        current->operation = child->operation;
        children = std::move(child->children);

        tree.destroy_node(child);
    }
    else
    {
        for (std::size_t child_i{0}; child_i < children.size(); ++child_i)
        {
            auto child = children[child_i];
            if (child->operation == current->operation && !child->is_leaf())
            {
                for (auto childs_child : child->children)
                {
                    childs_child->parent = current;
                }
                children.erase(std::remove(children.begin(), children.end(), child), children.end());
                children.insert(children.end(), child->children.begin(), child->children.end());

                tree.destroy_node(child);
                --child_i; // compensate for removed child
            }
            else
            {
                // in an else because we dont wanna check destroyed child!
                remove_unneeded_braces_recursion(tree, child);
            }
        }
    }
}
