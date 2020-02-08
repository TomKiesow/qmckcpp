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

    auto table_str = std::string(std::istreambuf_iterator<char>(table_file_stream), std::istreambuf_iterator<char>());

    char const *begin = table_str.c_str();
    char const *end = table_str.c_str() + table_str.size();

    auto table = parse_logic_table(begin, end);
    auto qtable = quine_table(table);

    auto result = deduce(qtable);

    std::vector<std::size_t> all_minterms;
    for (const auto &pair : result.rows)
    {
        const auto &row = pair.second;
        const auto &minterms = row.calculate_minterms_covered();
        all_minterms.insert(all_minterms.end(), minterms.begin(), minterms.end());

        std::sort(all_minterms.begin(), all_minterms.end());
        all_minterms.erase(std::unique(all_minterms.begin(), all_minterms.end()), all_minterms.end());
    }

    auto indices = std::vector<std::size_t>(all_minterms.size(), 0);

    auto chart = std::vector<std::vector<bool>>(all_minterms.size());
    for (auto &row : chart)
    {
        row.resize(result.rows.size(), false);
    }

    for (const auto &pair : result.rows)
    {
        const auto &row = pair.second;
        const auto &id = pair.first;
        const auto &minterms = row.calculate_minterms_covered();

        for (const auto &minterm : minterms)
        {
            auto index = std::distance(all_minterms.begin(), std::find(all_minterms.begin(), all_minterms.end(), minterm));
            chart[index][id] = true;
        }
    }

    for (std::size_t y = 0; y < chart[0].size(); ++y)
    {
        for (std::size_t x = 0; x < chart.size(); ++x)
        {
            if (chart[x][y])
            {
                std::cout << " x ";
            }
            else
            {
                std::cout << " _ ";
            }
        }
        std::cout << std::endl;
    }

    // remove columns by necessary implicants
    for (std::size_t col_i = 0; col_i < chart.size(); ++col_i)
    {
        const auto &column = chart[col_i];
        const auto true_count = std::count(column.begin(), column.end(), true);
        if(true_count == 1){

        }
    }

    //    for (auto index : all_minterms)
    //    {
    //        std::cout << index << ", ";
    //    }
    //
    //    std::cout << "\n\n" << quine_table;

    return 0;
}
