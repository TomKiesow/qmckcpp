#include <qmck.hpp>

#include <fstream>
#include <iostream>

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
    std::cout << table;

    auto deduced = qmck::deduce(table);
    std::cout << deduced << '\n';

    return 0;
}
