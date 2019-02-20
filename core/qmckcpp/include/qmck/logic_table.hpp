#pragma once

#include <qmck/logic_array.hpp>
#include <qmck/logic_table_format.hpp>

#include <cstdint>
#include <string>

namespace qmck
{
    struct logic_table
    {
    public:
        logic_table_format format{};
        logic_array inputs{};
        logic_array outputs{};
        logic_array output_dc_masks{};

    public:
        logic_table(logic_table_format table_format, logic_array &&inputs, logic_array &&outputs, logic_array &&output_masks);

        logic_table(logic_table_format table_format, logic_array inputs, logic_array outputs, logic_array output_masks);

        logic_table(logic_table_format table_format, logic_array &&inputs, logic_array &&outputs);

        logic_table(logic_table_format table_format, logic_array inputs, logic_array outputs);

        logic_table() = default;

        logic_table(logic_table const &lhs) = default;

        logic_table(logic_table &&rhs) noexcept = default;

    public:

        logic_table &operator=(logic_table lhs);

        friend void swap(logic_table &first, logic_table &second);
    };

    logic_table parse_logic_table(char const *table_c_str_begin, char const *table_c_str_end);
}
