#pragma once

#include <cstdint>

namespace qmck
{
    struct generic_table_format
    {
    public:
        std::uint8_t inputs_count{};
        std::uint8_t outputs_count{};

    public:
        generic_table_format(uint8_t inputs_count, uint8_t outputs_count);

        generic_table_format() = default;

        generic_table_format(generic_table_format const &lhs) = default;

        generic_table_format(generic_table_format &&rhs) = default;

    public:

        generic_table_format &operator=(generic_table_format lhs);

        friend void swap(generic_table_format &first, generic_table_format &second);
    };
}
