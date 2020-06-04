#pragma once

#include <cstdint>

namespace qmck
{
    struct generic_table_format
    {
    public:

        uint8_t inputs_count{};
        uint8_t outputs_count{};

    public:

        constexpr generic_table_format() noexcept = default;

        constexpr generic_table_format(generic_table_format const &lhs) noexcept = default;

        constexpr generic_table_format(generic_table_format &&rhs) noexcept = default;

        constexpr generic_table_format(uint8_t inputs_count, uint8_t outputs_count) noexcept: inputs_count(inputs_count), outputs_count(outputs_count)
        {
        }

    public:

        constexpr generic_table_format &operator=(const generic_table_format &) noexcept = default;

        constexpr generic_table_format &operator=(generic_table_format &&) noexcept = default;
    };
}
