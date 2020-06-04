#pragma once

#include <qmck/logic_value.hpp>
#include <qmck/generic_table_format.hpp>

#include <vector>
#include <cstdlib>

namespace qmck
{
    struct basic_row
    {
    public:

        logic_value inputs{};
        logic_value inputs_deduced_mask{};
        logic_value outputs{};

    public:

        constexpr basic_row() noexcept = default;

        constexpr basic_row(const basic_row &other) noexcept = default;

        constexpr basic_row(basic_row &&other) noexcept = default;

    public:

        constexpr basic_row &operator=(const basic_row &) noexcept = default;

        constexpr basic_row &operator=(basic_row &&) noexcept = default;

    public:

        [[nodiscard]] std::vector<logic_value> calculate_minterms_covered() const noexcept
        {
            std::vector<logic_value> result{};
            result.push_back(inputs & ~inputs_deduced_mask); // value with all deduced bits as 0

            const size_t n = sizeof(logic_value) * 8;
            for (size_t i = 0; i < n; ++i)
            {
                if (((inputs_deduced_mask >> i) & 1u) == 1u)
                {
                    logic_value diff = 1u << i;

                    size_t result_size = result.size();
                    for (size_t j = 0; j < result_size; ++j)
                    {
                        result.push_back(result[j] + diff);
                    }
                }
            }

            return result;
        }
    };
}
