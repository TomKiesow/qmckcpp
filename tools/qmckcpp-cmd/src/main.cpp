#include <qmck.hpp>
#include <qmck/tree/tree.hpp>

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

    auto tree = tree::build_tree(result, 0);

//    tree::remove_unneeded_braces(tree);

    std::cout << tree << "\n";

//    for (int i = 0; i < 6; ++i)
//    {
//        auto res = tree::multiply_nodes(&rootnode, rootnode.children[0], rootnode.children[1]);
//
//        auto child1 = rootnode.children[0];
//        auto child2 = rootnode.children[1];
//
//        rootnode.disconnect_child(child1);
//        rootnode.disconnect_child(child2);
//
//        rootnode.remove_node(child1);
//        rootnode.remove_node(child2);
//
//        rootnode.connect_child(res);
//
//        std::cout << rootnode << "\n";
//    }

    return 0;
}
