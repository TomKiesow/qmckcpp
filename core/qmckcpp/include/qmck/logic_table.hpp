#pragma once

#include <qmck/logic_row.hpp>
#include <qmck/generic_table_format.hpp>

#include <vector>

namespace qmck
{
    struct logic_table
    {
    public:
        generic_table_format format{};
        std::vector<logic_row> rows{};

    public:

        logic_table() noexcept = default;

        logic_table(logic_table const &) noexcept = default;

        logic_table(logic_table &&) noexcept = default;

    public:

        logic_table &operator=(const logic_table &) noexcept = default;

        logic_table &operator=(logic_table &&) noexcept = default;
    };

    logic_table parse_logic_table(char const *table_c_str_begin, char const *table_c_str_end);
}
