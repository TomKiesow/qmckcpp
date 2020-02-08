#include <qmck.hpp>
#include <qmck_optional_output.hpp>

#include <list>
#include <iostream>

namespace qmck
{
    qmck::result_table deduce(const quine_table &table)
    {
        quine_table quine_table_orig = table;

        auto &quine_table_current = quine_table_orig;
        quine_table quine_table_next(quine_table_current.format);

        quine_table all_prime_rows(quine_table_current.format);

        QMCK_OUTPUT_PROGRESS(int quine_table_counter = 2;) // number 1 is the original

        auto ranks_size = quine_table_current.ranks.size();
        while (!quine_table_current.empty())
        {
            QMCK_OUTPUT_PROGRESS(std::size_t comparison_max = quine_table_current.calculate_comparison_count_max();)
            QMCK_OUTPUT_PROGRESS(std::size_t comparison_current = 0;)

            for (std::size_t i = 0; i < ranks_size; ++i)
            {
                auto &result_bundle = quine_table_next.ranks[i];

                auto &lower_bundle = quine_table_current.ranks[i];
                auto &upper_bundle = quine_table_current.ranks[i + 1];

                auto lower_n = lower_bundle.size();
                for (std::size_t lower_i = 0; lower_i < lower_n; ++lower_i)
                {
                    auto &lower_row = lower_bundle[lower_i];

                    auto upper_n = upper_bundle.size();
                    for (std::size_t upper_i = 0; upper_i < upper_n; ++upper_i)
                    {
                        QMCK_OUTPUT_PROGRESS(if (comparison_current % 10000000 == 0)
                        {
                            std::cout << "\rtable of order " << quine_table_counter << ": " << (int) ((double) comparison_current / comparison_max * 100) << "%";
                        })
                        QMCK_OUTPUT_PROGRESS(++comparison_current;)

                        auto &upper_row = upper_bundle[upper_i];

                        if (lower_row.inputs_deduced_mask == upper_row.inputs_deduced_mask && __builtin_popcount(lower_row.inputs ^ upper_row.inputs) == 1 && lower_row.outputs & upper_row.outputs)
                        {
                            quine_row next_row;

                            next_row.inputs_deduced_mask = (lower_row.inputs ^ upper_row.inputs) | lower_row.inputs_deduced_mask;
                            next_row.inputs = lower_row.inputs & upper_row.inputs;
                            next_row.outputs_dc_mask = lower_row.outputs_dc_mask & upper_row.outputs_dc_mask;
                            next_row.outputs = lower_row.outputs & upper_row.outputs;
                            next_row.outputs_done_mask = next_row.outputs_dc_mask;

                            result_bundle.push_back(next_row);

                            lower_row.outputs_done_mask |= next_row.outputs;
                            upper_row.outputs_done_mask |= next_row.outputs;
                        }
                    }
                }
            }

            // go through each row in quine_table_current
            // and add it to all_prime_rows in case it is not completely covered
            // and there is no other equal row in there already
            for (std::size_t rank_i = 0; rank_i < ranks_size; ++rank_i)
            {
                auto &rank = quine_table_current.ranks[rank_i];

                for (std::size_t j = 0; j < rank.size(); ++j)
                {
                    auto &new_row = rank[j];
                    if ((new_row.outputs & new_row.outputs_done_mask) != new_row.outputs)
                    {
                        // check if an equal row is already in results table
                        bool is_duplicate = false;
                        auto &target_rank = all_prime_rows.ranks[rank_i];

                        for (std::size_t i = 0; i < target_rank.size(); ++i)
                        {
                            auto &old_row = target_rank[i];
                            if (new_row.inputs_deduced_mask == old_row.inputs_deduced_mask && (new_row.inputs & ~new_row.inputs_deduced_mask) == (old_row.inputs & ~old_row.inputs_deduced_mask))
                            {
                                is_duplicate = true;
                                break;
                            }
                        }
                        if (!is_duplicate)
                        {
                            target_rank.push_back(new_row);
                        }
                    }
                }
            }
            quine_table_current = quine_table_next;
            quine_table_next = quine_table(quine_table_current.format);

            QMCK_OUTPUT_PROGRESS(std::cout << "\rtable of order " << quine_table_counter << ": 100%\n";)
            QMCK_OUTPUT_PROGRESS(++quine_table_counter;)
        }

        return result_table(all_prime_rows);
    }
}
