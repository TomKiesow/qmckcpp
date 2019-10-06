#include <qmck/tree/tree_utils.hpp>
#include <qmck_io.hpp>

#include <algorithm>
#include <iostream>

qmck::tree::tree qmck::tree::utils::build_tree(const result_table &result, const int result_col)
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

    auto roots_children = rootnode->children;
    for (auto child : roots_children)
    {
        if (child->children.empty())
        {
            rootnode->remove_child(child);
            tree.destroy_node(child);
        }
    }

    return tree;
}

// true if v2 is subset of v1
bool qmck::tree::utils::is_sorted_subvector(const std::vector<node *> &v1, const std::vector<node *> &v2, bool (*equals_function)(const node *, const node *))
{
    std::size_t i1{0};
    std::size_t i2{0};

    if (v2.empty())
    {
        return true;
    }

    while (i1 < v1.size() && i2 < v2.size())
    {
        if (equals_function(v1[i1], v2[i2]))
        {
            ++i2;
        }
        else
        {
            ++i1;
        }
    }

    return i2 == v2.size();
}

// only use this for sorting, equal nodes might not be equal according to this function
bool qmck::tree::utils::node_comparator(const node *a, const node *b)
{
    if (a->is_leaf() && b->is_leaf())
    {
        return a->operand < b->operand;
    }
    if (!a->is_leaf() && b->is_leaf())
    {
        return false;
    }
    if (a->is_leaf() && !b->is_leaf())
    {
        return true;
    }
    if (!a->is_leaf() && !b->is_leaf())
    {
        return a->children.size() < b->children.size();
    }
    return false;
}

int qmck::tree::utils::calc_depth(const node *node)
{
    if (node->children.empty())
    {
        return 0;
    }

    std::vector<int> children_depths{};
    for (auto child : node->children)
    {
        children_depths.push_back(1 + calc_depth(child));
    }

    int max = 0;
    for (auto depth : children_depths)
    {
        if (depth > max)
        {
            max = depth;
        }
    }

    return max;
}

void qmck::tree::utils::remove_unneeded_braces(tree &tree)
{
    remove_unneeded_braces_recursion(tree, tree.rootnode);
}

void qmck::tree::utils::remove_unneeded_braces_recursion(tree &tree, node *current)
{
    auto &children = current->children;

    // if current has only one child
    // remove child and connect its children to self
    while (children.size() == 1)
    {
        auto child = children[0];
        for (auto childs_child : child->children)
        {
            childs_child->parent = current;
        }
        current->operand = child->operand;
        current->operation = child->operation;
        children = std::move(child->children);

        tree.destroy_node(child);
    }

    bool children_changed = true;
    while (children_changed)
    {
        children_changed = false;

        for (auto child : children)
        {
            remove_unneeded_braces_recursion(tree, child);
            if (child->operation == current->operation && !child->is_leaf())
            {
                for (auto childs_child : child->children)
                {
                    childs_child->parent = current;
                }
                children.erase(std::remove(children.begin(), children.end(), child), children.end());
                children.insert(children.end(), child->children.begin(), child->children.end());

                tree.destroy_node(child);
                // break loop and start over from the beginning
                // because newly added children would be ignored otherwise
                children_changed = true;
                break;
            }
        }
    }
}

void qmck::tree::utils::simplify_idempotency(tree &tree)
{
    simplify_idempotency_recursion(tree, tree.rootnode);
}

void qmck::tree::utils::simplify_idempotency_recursion(tree &tree, node *current)
{
    auto &children = current->children;
    std::sort(children.begin(), children.end(), node_comparator);

    for (auto child : children)
    {
        simplify_idempotency_recursion(tree, child);
    }

    for (std::size_t child_i{0}; child_i < children.size(); ++child_i)
    {
        auto child = children[child_i];
        for (std::size_t sibling_i{child_i + 1}; sibling_i < children.size(); ++sibling_i)
        {
            auto sibling = children[sibling_i];
            if (child->children.size() == sibling->children.size())
            {
                if (child->is_leaf())
                {
                    // if nodes share operand we can remove one of them
                    if (sibling->is_leaf() && child->operand == sibling->operand && child->negated == sibling->negated)
                    {
                        children.erase(children.begin() + sibling_i);
                        --sibling_i;
                        tree.destroy_node(sibling);
                    }
                }
                else
                {
                    if (is_sorted_subvector(child->children, sibling->children, node::equals))
                    {
                        children.erase(children.begin() + sibling_i);
                        --sibling_i;
                        tree.destroy_subtree(sibling);
                    }
                }
            }
        }
    }
}

void qmck::tree::utils::simplify_absorption(tree &tree)
{
    // TODO: absorbtion is whats holding us back. change this to pass newly created nodes explicitly so that we dont check each node 1000 times!
    simplify_absorption(tree, tree.rootnode);
}

bool qmck::tree::utils::simplify_absorption(tree &tree, node *node)
{
    std::sort(node->children.begin(), node->children.end(), node_comparator);
    auto children_todo = node->children;

    while(!children_todo.empty())
    {
        auto child = children_todo[0];

        std::sort(child->children.begin(), child->children.end(), node_comparator);

        auto child_depth = calc_depth(child);
        if (child_depth <= 1) // child could absorb something...
        {
            for (std::size_t sibling_i{1}; sibling_i < children_todo.size(); ++sibling_i)
            {
                auto sibling = children_todo[sibling_i];

                bool absorb = false;
                if (child->is_leaf())
                {
                    for (auto siblings_child : sibling->children)
                    {
                        if (siblings_child->is_leaf() && siblings_child->operand == child->operand && siblings_child->negated == child->negated)
                        {
                            absorb = true;
                            break;
                        }
                    }
                }
                else
                {
                    std::sort(sibling->children.begin(), sibling->children.end(), node_comparator);
                    absorb = is_sorted_subvector(sibling->children, child->children, node::equals);
                }

                if (absorb)
                {
                    node->remove_child(sibling);
                    children_todo.erase(std::remove(children_todo.begin(), children_todo.end(), sibling), children_todo.end());
                    tree.destroy_subtree(sibling);
                    --sibling_i;
                }
            }
        }
        if (child_depth > 1)
        {
            simplify_absorption(tree, child);
        }
        children_todo.erase(children_todo.begin());
    }

    return false;
}

qmck::tree::node *qmck::tree::utils::duplicate_subtree(tree &tree, node *orig)
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

qmck::tree::node *qmck::tree::utils::multiply_nodes(tree &tree, node *parent_of_both, node *node1, node *node2)
{
    auto result = tree.create_node(nullptr, 0, parent_of_both->operation);

    // if one of the two nodes is a leaf node
    if (node1->is_leaf() != node2->is_leaf())
    {
        node *leaf = node1->is_leaf() ? node1 : node2;
        node *subtree = node1->is_leaf() ? node2 : node1;

        for (auto child : subtree->children)
        {
            auto new_node = tree.create_node(result, 0, result->operation);
            result->children.push_back(new_node);

            child->parent = new_node;
            new_node->children.push_back(child);

            auto leaf_copy = tree.create_node(new_node, leaf->operand, leaf->operation); // operation actually irrelevant
            new_node->children.push_back(leaf_copy);
        }

        parent_of_both->remove_child(leaf);
        parent_of_both->remove_child(subtree);

        result->operation = subtree->operation;

        tree.destroy_node(leaf);
        tree.destroy_node(subtree);

        result->parent = parent_of_both;
        parent_of_both->children.push_back(result);
    }

        // both nodes are subtrees
    else if (!node1->is_leaf() && !node2->is_leaf())
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

        result->operation = node1->operation; // or node2->operation

        tree.destroy_subtree(node1);
        tree.destroy_subtree(node2);

        result->parent = parent_of_both;
        parent_of_both->children.push_back(result);
    }

    return result;
}

bool qmck::tree::utils::has_petrick_result_form(const tree &tree)
{
    auto &children = tree.rootnode->children;

    for (auto child : children)
    {
        if (!child->is_leaf())
        {
            if (child->operation == OPERATION_OR)
            {
                return false;
            }
        }
    }

    return calc_depth(tree.rootnode) <= 2;
}

std::vector<qmck::logic_value> qmck::tree::utils::get_all_operands(const node *node)
{
    std::vector<qmck::logic_value> result{};

    get_all_operands_recursion(node, result);

    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

void qmck::tree::utils::get_all_operands_recursion(const node* current, std::vector<qmck::logic_value> &values){
    if(current->is_leaf()){
        values.push_back(current->operand);
    }
    for(auto child : current->children){
        get_all_operands_recursion(child, values);
    }
}
