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

    auto table = qmck::parse_logic_table(begin, end);
    table.sort();
    std::cout << table << '\n';

    auto result = qmck::deduce(table);
    std::cout << result << "\n\n";

    auto rootnode = qmck::tree::build_tree(result, 0);

    std::cout << rootnode << "\n";

    qmck::tree::remove_unneeded_braces(rootnode);

    std::cout << rootnode << "\n";

    return 0;
}
