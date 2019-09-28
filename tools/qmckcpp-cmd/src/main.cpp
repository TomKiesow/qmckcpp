#include <qmck.hpp>
#include <qmck/tree/tree_utils.hpp>
#include <loguru.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <algorithm>

using namespace qmck;

int main(int argc, char **argv)
{
    loguru::g_preamble_thread = false;
    loguru::g_preamble_uptime = false;

#ifdef DEBUG
    #define LOGURU_LOG_ARGS_AND_DIR 1
    loguru::g_preamble_file = true;
    loguru::g_preamble_verbose = true;
    loguru::g_stderr_verbosity = 9; // everything
#else
    loguru::g_preamble_verbose = false;
    loguru::g_preamble_file = false;
    loguru::g_stderr_verbosity = 0; // only info, will get overwritten by -v n
#endif

    loguru::init(argc, argv);

    if (argc < 2)
    {
        LOG_F(ERROR, "no input file specified");
        return -1;
    }

    std::ifstream table_file_stream(argv[1]);

    if (table_file_stream.fail())
    {
        LOG_F(ERROR, "%s: could not open/find file", argv[1]);
        return -1;
    }

    std::string table_str{std::istreambuf_iterator<char>(table_file_stream), std::istreambuf_iterator<char>()};

    char const *begin = table_str.c_str();
    char const *end = table_str.c_str() + table_str.size();

    auto table = parse_logic_table(begin, end);

    auto result = deduce(table);

    std::vector<qmck::logic_value> operands{};

    for (logic_value output_index{0}; output_index < result.format.outputs_count; ++output_index)
    {
        std::cout << "solutions for output no." << output_index << ":\n";
        auto tree = tree::utils::build_tree(result, output_index);

        tree::utils::remove_unneeded_braces(tree);
        tree::utils::simplify_absorption(tree);
        tree::utils::simplify_idempotency(tree);
        tree::utils::remove_unneeded_braces(tree);

        auto &children = tree.rootnode->children;
        while (!tree::utils::has_petrick_result_form(tree) && children.size() >= 2)
        {
            auto child1 = children[0];
            auto child2 = children[1];
            // make sure child1 or child2 is not a leaf
            // otherwise multiplication would not change anything
            if (child1->is_leaf() && child2->is_leaf())
            {
                for (std::size_t child_i{2}; child_i < children.size(); ++child_i)
                {
                    if (!children[child_i]->is_leaf())
                    {
                        child2 = children[child_i];
                        break;
                    }
                }
            }

            tree::utils::multiply_nodes(tree, tree.rootnode, child1, child2);
            tree::utils::remove_unneeded_braces(tree);
            tree::utils::simplify_idempotency(tree);
            tree::utils::remove_unneeded_braces(tree);
            tree::utils::simplify_absorption(tree);
            tree::utils::remove_unneeded_braces(tree);
        }

        // only one solution
        if (tree.rootnode->operation == tree::OPERATION_AND)
        {
            auto solution = tree.rootnode;
            auto outputs_operands = tree::utils::get_all_operands(solution);
            operands.insert(operands.end(), outputs_operands.begin(), outputs_operands.end());

            int cost{0};
            for (auto outputs_operand : outputs_operands)
            {
                cost += result.rows[outputs_operand].cost;
            }

            std::cout << solution << " (cost: " << cost << ")\n\n";
        }
        // multiple solutions
        else
        {
            for (auto solution : children)
            {
                auto outputs_operands = tree::utils::get_all_operands(solution);
                operands.insert(operands.end(), outputs_operands.begin(), outputs_operands.end());

                int cost{0};
                for (auto outputs_operand : outputs_operands)
                {
                    cost += result.rows[outputs_operand].cost;
                }

                std::cout << solution << " (cost: " << cost << ")\n";
            }
            std::cout << '\n';
        }
    }

    std::sort(operands.begin(), operands.end());
    operands.erase(std::unique(operands.begin(), operands.end()), operands.end());
    for (auto operand : operands)
    {
        std::cout << operand << ": ";
        print_result_row(std::cout, result.rows[operand], result.format);
        std::cout << '\n';
    }

    return 0;
}
