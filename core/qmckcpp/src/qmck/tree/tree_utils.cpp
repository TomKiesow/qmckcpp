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

void qmck::tree::utils::simplify_idempotency(qmck::tree::tree &tree)
{
    auto &all_nodes = tree.all_nodes;
    for (std::size_t all_nodes_i{0}; all_nodes_i < all_nodes.size(); ++all_nodes_i)
    {
        auto current_node = all_nodes[all_nodes_i].get();
        auto &children = current_node->children;
        for (std::size_t child_i1{0}; child_i1 < children.size(); ++child_i1)
        {
            auto child1 = children[child_i1];
            if (child1->is_leaf())
            {
                for (std::size_t child_i2{child_i1 + 1}; child_i2 < children.size(); ++child_i2)
                {
                    auto child2 = children[child_i2];
                    // if nodes share operand we ca remove one of them
                    if(child2->is_leaf() && child1->operand == child2->operand && child1->negated == child2->negated){
                        current_node->remove_child(child2);
                        --child_i2; // compensate for removed child
                        tree.destroy_node(child2);
                    }
                }
            }
        }
    }
}

qmck::tree::node *qmck::tree::utils::duplicate_subtree(qmck::tree::tree &tree, qmck::tree::node *orig)
{
    qmck::tree::node *copy = tree.create_node(nullptr, orig->operand, orig->operation);

    for (auto child : orig->children)
    {
        auto child_copy = duplicate_subtree(tree, child);

        child_copy->parent = copy;
        copy->children.push_back(child_copy);
    }

    return copy;
}

qmck::tree::node *qmck::tree::utils::multiply_nodes(qmck::tree::tree &tree, qmck::tree::node *parent_of_both, qmck::tree::node *node1, qmck::tree::node *node2)
{
    auto result = tree.create_node(nullptr, 0, parent_of_both->operation);

    // if one of the two nodes is a leaf node
    if (node1->is_leaf() != node2->is_leaf())
    {
        node *leaf = node1->is_leaf() ? node1 : node2;
        node *subtree = node1->is_leaf() ? node2 : node1;

        for (std::size_t child_i{0}; child_i < subtree->children.size(); ++child_i)
        {
            auto child = subtree->children[child_i];

            auto new_node = tree.create_node(result, 0, result->operation);
            result->children.push_back(new_node);

            subtree->remove_child(child);
            --child_i; // compensate for removed child

            child->parent = new_node;
            new_node->children.push_back(child);

            auto leaf_copy = tree.create_node(new_node, leaf->operand, leaf->operation); // operation actually irrelevant
            new_node->children.push_back(leaf_copy);
        }
        parent_of_both->remove_child(leaf);
        parent_of_both->remove_child(subtree);

        tree.destroy_node(leaf);
        tree.destroy_node(subtree);

        result->operation = subtree->operation;

        result->parent = parent_of_both;
        parent_of_both->children.push_back(result);
    }

        // both nodes are subtrees
    else
    {
        for (auto child1 : node1->children)
        {
            for (auto child2: node2->children)
            {
                auto new_node = tree.create_node(result, 0, result->operation);
                result->children.push_back(new_node);

                auto child1_copy = duplicate_subtree(tree, child1);
                auto child2_copy = duplicate_subtree(tree, child2);

                child1_copy->parent = new_node;
                child2_copy->parent = new_node;

                new_node->children.push_back(child1_copy);
                new_node->children.push_back(child2_copy);
            }
        }

        parent_of_both->remove_child(node1);
        parent_of_both->remove_child(node2);

        tree.destroy_subtree(node1);
        tree.destroy_subtree(node2);

        result->operation = node1->operation; // or node2->operation

        result->parent = parent_of_both;
        parent_of_both->children.push_back(result);
    }

    return result;
}
