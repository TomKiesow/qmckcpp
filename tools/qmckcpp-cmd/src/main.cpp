#include <qmck.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <algorithm>

using namespace qmck;

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        std::cout << "no input file specified\n";
        return -1;
    }

    std::ifstream table_file_stream(argv[1]);

    if (table_file_stream.fail())
    {
        std::cout << argv[1] << ": could not open/find file\n";
        return -1;
    }

    std::string table_str{std::istreambuf_iterator<char>(table_file_stream), std::istreambuf_iterator<char>()};

    char const *begin = table_str.c_str();
    char const *end = table_str.c_str() + table_str.size();

    auto table = parse_logic_table(begin, end);

    auto result = deduce(table);

    return 0;
}
