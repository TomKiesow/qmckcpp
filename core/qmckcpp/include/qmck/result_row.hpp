#pragma once

#include <qmck/basic_row.hpp>
#include <qmck/logic_value.hpp>
#include <qmck/generic_table_format.hpp>
#include <qmck/quine_row.hpp>

#include <vector>

namespace qmck
{
    struct result_row : basic_row
    {
    public:
        int cost{};

    public:
        result_row() = default;

        explicit result_row(const quine_row &lrow, const generic_table_format &format);

    private:
        int calculate_cost(const generic_table_format &format) const;
    };
}
