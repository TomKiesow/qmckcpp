#pragma once

#include <qmck/generic_table_format.hpp>
#include <qmck/quine_row.hpp>
#include <qmck/logic_table.hpp>

#include <array>
#include <vector>

namespace qmck
{
    struct quine_table
    {
    public:
        generic_table_format format{};
        std::array<std::vector<quine_row>, 32> ranks;

    public:

        explicit quine_table(logic_table const &lhs);

        quine_table() = default;

        quine_table(quine_table const &lhs) = default;

        quine_table(quine_table &&lhs) noexcept = default;

        explicit quine_table(const generic_table_format &format);

    public:

        bool empty() const;

        quine_table &operator=(quine_table const &lhs) = default;

    };
}
