#include <qmck.hpp>

#include <iostream>
#include <vector>
#include <list>
#include <iterator>

namespace qmck
{
    qmck::quine_table deduce(qmck::logic_table &table)
    {
        quine_table quine_table_orig{table};

        auto &quine_table_current = quine_table_orig;
        quine_table quine_table_next{quine_table_current.format};

        quine_table all_prime_rows{quine_table_current.format};

        auto ranks_size = quine_table_current.ranks.size();
        while (!quine_table_current.empty())
        {
            for (size_t i{0}; i < ranks_size; ++i)
            {
                auto &next_bundle = quine_table_next.ranks[i];
                auto &lower_bundle = quine_table_current.ranks[i];
                auto &upper_bundle = quine_table_current.ranks[i + 1];

                auto lower_n = lower_bundle.size();
                for (size_t lower_i{0}; lower_i < lower_n; ++lower_i)
                {
                    auto &lower_row = lower_bundle[lower_i];

                    auto upper_n = upper_bundle.size();
                    for (size_t upper_i{0}; upper_i < upper_n; ++upper_i)
                    {
                        auto &upper_row = upper_bundle[upper_i];

                        if (lower_row.inputs_deduced_mask == upper_row.inputs_deduced_mask && __builtin_popcount(lower_row.inputs ^ upper_row.inputs) == 1 && lower_row.outputs & upper_row.outputs)
                        {
                            quine_row next_row;

                            next_row.inputs_deduced_mask = (lower_row.inputs ^ upper_row.inputs) | lower_row.inputs_deduced_mask;
                            next_row.inputs = lower_row.inputs & upper_row.inputs;
                            next_row.outputs_dc_mask = lower_row.outputs_dc_mask & upper_row.outputs_dc_mask;
                            next_row.outputs = lower_row.outputs & upper_row.outputs;
                            next_row.outputs_done_mask = next_row.outputs_dc_mask;

                            next_bundle.push_back(next_row);

                            lower_row.outputs_done_mask |= next_row.outputs;
                            upper_row.outputs_done_mask |= next_row.outputs;
                        }
                    }
                }
            }

            // go through each row in quine_table_current
            // and add it to all_prime_rows in case it is not completely covered
            for (int rank_i = 0; rank_i < ranks_size; ++rank_i)
            {
                auto &rank = quine_table_current.ranks[rank_i];

                for (int j = 0; j < rank.size(); ++j)
                {
                    if ((rank[j].outputs & rank[j].outputs_done_mask) != rank[j].outputs)
                    {
                        auto &target_bundle = all_prime_rows.ranks[rank_i];
                        target_bundle.push_back(rank[j]);
                    }
                }
            }
            quine_table_current = quine_table_next;
            quine_table_next = quine_table(quine_table_current.format);
        }


        // go through all_prime_rows and remove duplicates
        for (int rank_i = 0; rank_i < ranks_size; ++rank_i)
        {
            auto &rank = all_prime_rows.ranks[rank_i];

            for (int row_i_upper = 0; row_i_upper < rank.size(); ++row_i_upper)
            {
                auto &upper_inputs = rank[row_i_upper].inputs;
                auto &upper_inputs_deduced_masks = rank[row_i_upper].inputs_deduced_mask;

                for (int row_i_lower = 0; row_i_lower < rank.size(); ++row_i_lower)
                {
                    if (row_i_lower != row_i_upper)
                    {
                        auto &lower_inputs = rank[row_i_lower].inputs;
                        auto &lower_inputs_deduced_masks = rank[row_i_lower].inputs_deduced_mask;

                        if (upper_inputs_deduced_masks == lower_inputs_deduced_masks && (upper_inputs & ~upper_inputs_deduced_masks) == (lower_inputs & ~lower_inputs_deduced_masks))
                        {
                            rank.erase(rank.begin() + row_i_lower);
                            row_i_lower--; // compensate for deleted row, otherwise the row after the deleted one would be skipped
                        }
                    }
                }
            }
        }

        return all_prime_rows;
    }
}
