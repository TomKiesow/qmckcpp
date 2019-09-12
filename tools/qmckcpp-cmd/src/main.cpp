#include <qmck.hpp>
#include <qmck/tree/tree_utils.hpp>

#include <fstream>
#include <iostream>
#include <memory>

using namespace qmck;

int main(int const argc, char const **argv)
{
    if (argc < 2)
    {
        return -1;
    }

    std::ifstream table_file_stream(argv[1]);
    std::string table_str{std::istreambuf_iterator<char>(table_file_stream), std::istreambuf_iterator<char>()};

    char const *begin = table_str.c_str();
    char const *end = table_str.c_str() + table_str.size();

    auto table = parse_logic_table(begin, end);
    table.sort();
    std::cout << table << '\n';

    auto result = deduce(table);
    std::cout << result << "\n\n";

    auto tree = tree::utils::build_tree(result, 0);

    std::cout << tree << "\n";

    tree::utils::remove_unneeded_braces(tree);
    tree::utils::simplify_absorption(tree);

    std::cout << tree << "\n";

    while ((tree::utils::calc_depth(tree.rootnode) > 1 || tree.rootnode->operation != tree::OPERATION_OR) && tree.rootnode->children.size() >= 2)
    {
        tree::utils::multiply_nodes(tree, tree.rootnode, tree.rootnode->children[0], tree.rootnode->children[1]);
        tree::utils::remove_unneeded_braces(tree);
        tree::utils::simplify_idempotency(tree);
        tree::utils::remove_unneeded_braces(tree);
        tree::utils::simplify_absorption(tree);
        tree::utils::remove_unneeded_braces(tree);
        std::cout << tree << "\n";
    }

    return 0;
}
