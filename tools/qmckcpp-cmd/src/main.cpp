#include <qmck.hpp>

#include <fstream>
#include <iostream>
#include <memory>

using namespace qmck;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "no input file specified\n";
        return -1;
    }

    std::ifstream table_file_stream{argv[1]};

    if (table_file_stream.fail())
    {
        std::cout << argv[1] << ": could not open/find file\n";
        return -1;
    }

    const std::string table_str{std::istreambuf_iterator<char>(table_file_stream), std::istreambuf_iterator<char>()};

    const auto begin = table_str.c_str();
    const auto end = table_str.c_str() + table_str.size();

    const auto table = parse_logic_table(begin, end);
    const auto result = deduce(table);

    return 0;
}
