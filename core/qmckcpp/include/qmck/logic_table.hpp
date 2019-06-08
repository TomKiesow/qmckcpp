#pragma once

#include <qmck/logic_row.hpp>
#include <qmck/generic_table_format.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace qmck
{
    struct logic_table
    {
    public:
        generic_table_format format{};
        std::vector<logic_row> rows;

    public:
        logic_table() = default;

        logic_table(logic_table const &lhs) = default;

        logic_table(logic_table &&rhs) noexcept = default;

        void sort();

    public:

        logic_table &operator=(logic_table lhs);

        friend void swap(logic_table &first, logic_table &second);
    };

    logic_table parse_logic_table(char const *table_c_str_begin, char const *table_c_str_end);
}
