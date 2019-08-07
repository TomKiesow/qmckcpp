#pragma once

#include <qmck/logic_value.hpp>
#include <qmck/quine_row.hpp>
#include <qmck/quine_table.hpp>

#include <map>

namespace qmck
{
    struct result_table
    {
    public:
        generic_table_format format{};
        std::map<logic_value, quine_row> rows;

    public:
        result_table() = default;

        explicit result_table(quine_table &lhs);

    public:
        void add_row(quine_row &row);
    };
}