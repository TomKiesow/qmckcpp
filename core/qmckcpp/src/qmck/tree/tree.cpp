#include <qmck/tree/tree.hpp>

#include <algorithm>

qmck::tree::tree::tree()
{
    std::unique_ptr<node> new_rootnode = std::make_unique<node>(*this, 0, OPERATION_AND);
    this->rootnode = new_rootnode.get();
    all_nodes.push_back(std::move(new_rootnode));
}

qmck::tree::tree::tree(qmck::tree::tree &&other) noexcept
{
    this->rootnode = other.rootnode;
    other.rootnode = other.create_node(0, OPERATION_AND); // tree without rootnode is invalid
    this->all_nodes = std::move(other.all_nodes);
}

qmck::tree::node *qmck::tree::tree::create_node(qmck::logic_value operand, bool operation)
{
    std::unique_ptr<node> new_node = std::make_unique<node>(*this, operand, operation);
    node *new_node_ptr = new_node.get();
    all_nodes.push_back(std::move(new_node));

    return new_node_ptr;
}

void qmck::tree::tree::destroy_node(qmck::tree::node *target)
{
    all_nodes.erase(std::remove_if(all_nodes.begin(), all_nodes.end(), [target](std::unique_ptr<node> &node)
    {
        return node.get() == target;
    }), all_nodes.end());
}

qmck::tree::tree qmck::tree::build_tree(const qmck::result_table &result, int result_col)
{
    tree tree{};
    auto rootnode = tree.rootnode;
    for (std::size_t i{0}; i < (1u << result.format.inputs_count); i++)
    {
        auto new_node = tree.create_node(0, OPERATION_OR);
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

                auto new_node = tree.create_node(id, OPERATION_AND); // OPERATION_AND irrelevant because node is leaf!
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

int qmck::tree::calc_depth(node *node)
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

//qmck::tree::basic_node *qmck::tree::multiply_nodes(root_node *root, basic_node *node1, basic_node *node2)
//{
//    auto result_parent_unique_ptr = std::make_unique<internal_node>(*root, 0);
//    auto result_parent_ptr = result_parent_unique_ptr.get();
//
//    root->add_node(std::move(result_parent_unique_ptr));
//
//    for (auto &n1 : node1->children)
//    {
//        for (auto &n2 : node2->children)
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
//
//    return result_parent_ptr;
//}
