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
    private:

        uint8_t cost{};

    public:

        constexpr result_row() noexcept = default;

        constexpr result_row(const result_row &) noexcept = default;

        constexpr result_row(result_row &&) noexcept = default;

        explicit result_row(const quine_row &row, const generic_table_format &format) noexcept
        {
            outputs = row.outputs;
            cost = calculate_cost(format);
        }

    public:

        constexpr result_row &operator=(const result_row &) noexcept = default;

        constexpr result_row &operator=(result_row &&) noexcept = default;

    private:

        [[nodiscard]] uint8_t calculate_cost(const generic_table_format &format) const
        {
            uint8_t c = 0;

            const uint8_t n = format.inputs_count;
            for (uint8_t i = 0; i < n; ++i)
            {
                if (!((inputs_deduced_mask >> (format.inputs_count - i - 1u)) & 1u))
                {
                    ++c;
                }
            }
            return c;
        }
    };
}
