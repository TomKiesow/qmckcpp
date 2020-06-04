#pragma once

#include <qmck/logic_value.hpp>
#include <qmck/quine_row.hpp>
#include <qmck/result_row.hpp>
#include <qmck/quine_table.hpp>

#include <map>

namespace qmck
{
    struct result_table
    {
    public:

        generic_table_format format{};
        std::map<logic_value, result_row> rows{};

    public:

        result_table() noexcept = default;

        result_table(const result_table &) noexcept = default;

        result_table(result_table &&) noexcept = default;

        explicit result_table(const quine_table &lhs) noexcept
        {
            format = lhs.format;

            for (const auto &rank : lhs.ranks)
            {
                for (const auto &row : rank)
                {
                    add_row(result_row(row, format));
                }
            }
        }

    public:

        result_table &operator=(const result_table &) noexcept = default;

        result_table &operator=(result_table &&) noexcept = default;

    public:

        void add_row(result_row &&row) noexcept
        {
            rows[static_cast<unsigned int>(rows.size())] = row;
        }
    };
}
