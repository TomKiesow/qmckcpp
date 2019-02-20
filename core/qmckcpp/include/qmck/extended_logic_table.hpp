#pragma once

#include <qmck/logic_table.hpp>
#include <qmck/extended_logic_bundle.hpp>

#include <array>

namespace qmck
{
    struct extended_logic_table
    {
    public:
        logic_table_format format{};
        std::array<extended_logic_bundle, 32> logic_bundle_ranks;

    public:

        explicit extended_logic_table(logic_table const &lhs);

        explicit extended_logic_table(logic_table &&lhs);

        extended_logic_table() = default;

        extended_logic_table(extended_logic_table const &lhs) = default;

        extended_logic_table(extended_logic_table &&lhs) noexcept = default;

    public:

        extended_logic_table &operator=(extended_logic_table lhs);

        friend void swap(extended_logic_table &first, extended_logic_table &second);
    };
}
