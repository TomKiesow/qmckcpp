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

        generic_table_format format;
        std::array<std::vector<quine_row>, 32> ranks;

    public:

        quine_table() noexcept = default;

        quine_table(const quine_table &) = default;

        quine_table(quine_table &&) noexcept = default;

        explicit quine_table(const generic_table_format &format) noexcept: format(format)
        {
        }

        explicit quine_table(const logic_table &lhs) noexcept
        {
            format = lhs.format;

            for (auto &lrow : lhs.rows)
            {
                if (lrow.outputs)
                {
                    auto qrow = quine_row(lrow);

                    int rank_i = __builtin_popcount(qrow.inputs);
                    ranks[rank_i].push_back(qrow);
                }
            }
        }

    public:

        quine_table &operator=(const quine_table &) = default;

        quine_table &operator=(quine_table &&) = default;

    public:

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            for (auto &rank : ranks)
            {
                if (!rank.empty())
                {
                    return false;
                }
            }

            return true;
        }

        [[nodiscard]] constexpr size_t calculate_comparison_count_max() const noexcept
        {
            size_t count = 0;

            const size_t n = ranks.size() - 1;
            for (size_t i = 0; i < n; ++i)
            {
                auto &rank1 = ranks[i];
                auto &rank2 = ranks[i + 1];

                count += rank1.size() * rank2.size();
            }

            return count;
        }
    };
}
