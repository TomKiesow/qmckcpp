#include <qmck.hpp>

#include <iostream>
#include <vector>
#include <list>
#include <iterator>

namespace qmck
{
    qmck::extended_logic_table deduce(qmck::logic_table table)
    {
        qmck::extended_logic_table extended_table{table};

        auto &extended_table_current = extended_table;
        extended_logic_table extended_table_next{extended_table_current.format};

        extended_logic_table all_prime_rows{extended_table_current.format};

        while (!extended_table_current.empty())
        {
            auto n = extended_table_current.logic_bundle_ranks.size() - 1;
            for (size_t i{0}; i < n; ++i)
            {
                auto &next_bundle = extended_table_next.logic_bundle_ranks[i];
                auto &lower_bundle = extended_table_current.logic_bundle_ranks[i];
                auto &upper_bundle = extended_table_current.logic_bundle_ranks[i + 1];

                auto lower_n = lower_bundle.inputs.size();
                for (size_t lower_i{0}; lower_i < lower_n; ++lower_i)
                {
                    auto upper_n = upper_bundle.inputs.size();
                    for (size_t upper_i{0}; upper_i < upper_n; ++upper_i)
                    {
                        auto &lower_input = lower_bundle.inputs[lower_i];
                        auto &lower_input_deduced_mask = lower_bundle.input_deduced_masks[lower_i];
                        auto &lower_output = lower_bundle.outputs[lower_i];
                        auto &lower_output_dc_mask = lower_bundle.output_dc_masks[lower_i];
                        auto &lower_output_done_mask = lower_bundle.output_done_masks[lower_i];

                        auto &upper_input = upper_bundle.inputs[upper_i];
                        auto &upper_input_deduced_mask = upper_bundle.input_deduced_masks[upper_i];
                        auto &upper_output = upper_bundle.outputs[upper_i];
                        auto &upper_output_dc_mask = upper_bundle.output_dc_masks[upper_i];
                        auto &upper_output_done_mask = upper_bundle.output_done_masks[upper_i];

                        if (lower_input_deduced_mask == upper_input_deduced_mask && __builtin_popcount(lower_input ^ upper_input) == 1 && lower_output & upper_output)
                        {
                            uint32_t next_input{0};
                            uint32_t next_input_deduced_mask{0};
                            uint32_t next_output{0};
                            uint32_t next_output_dc_mask{0};
                            uint32_t next_output_done_mask{0};

                            next_input_deduced_mask = (lower_input ^ upper_input) | lower_input_deduced_mask;
                            next_input = lower_input & upper_input;
                            next_output_dc_mask = lower_output_dc_mask & upper_output_dc_mask;
                            next_output = lower_output & upper_output;
                            next_output_done_mask = next_output_dc_mask;

                            next_bundle.inputs.push_back(next_input);
                            next_bundle.input_deduced_masks.push_back(next_input_deduced_mask);
                            next_bundle.outputs.push_back(next_output);
                            next_bundle.output_dc_masks.push_back(next_output_dc_mask);
                            next_bundle.output_done_masks.push_back(next_output_done_mask);

                            lower_output_done_mask |= next_output;
                            upper_output_done_mask |= next_output;
                        }
                    }
                }
            }

            // go through each row in extended_table_current
            // and add it to all_prime_rows in case it is not completely covered
            for (int rank_i = 0; rank_i < 32; ++rank_i)
            {
                auto &rank = extended_table_current.logic_bundle_ranks[rank_i];

                for (int j = 0; j < rank.inputs.size(); ++j)
                {
                    if ((rank.outputs[j] & rank.output_done_masks[j]) != rank.outputs[j])
                    {
                        auto &target_bundle = all_prime_rows.logic_bundle_ranks[rank_i];

                        target_bundle.inputs.push_back(rank.inputs[j]);
                        target_bundle.input_deduced_masks.push_back(rank.input_deduced_masks[j]);
                        target_bundle.outputs.push_back(rank.outputs[j]);
                        target_bundle.output_dc_masks.push_back(rank.output_dc_masks[j]);
                        target_bundle.output_done_masks.push_back(rank.output_done_masks[j]);
                    }
                }
            }

            extended_table_current = extended_table_next;
            extended_table_next = extended_logic_table(extended_table_current.format);
        }

        // go through all_prime_rows and remove duplicates
        for (int rank_i = 0; rank_i < 32; ++rank_i)
        {
            auto &rank = all_prime_rows.logic_bundle_ranks[rank_i];

            for (int row_i_upper = 0; row_i_upper < rank.inputs.size(); ++row_i_upper)
            {
                auto &upper_inputs = rank.inputs[row_i_upper];
                auto &upper_inputs_deduced_masks = rank.input_deduced_masks[row_i_upper];

                for (int row_i_lower = 0; row_i_lower < rank.inputs.size(); ++row_i_lower)
                {
                    if (row_i_lower != row_i_upper)
                    {
                        auto &lower_inputs = rank.inputs[row_i_lower];
                        auto &lower_inputs_deduced_masks = rank.input_deduced_masks[row_i_lower];

                        if (upper_inputs_deduced_masks == lower_inputs_deduced_masks && (upper_inputs & ~upper_inputs_deduced_masks) == (lower_inputs & ~lower_inputs_deduced_masks))
                        {
                            rank.inputs.erase(rank.inputs.begin() + row_i_lower);
                            rank.input_deduced_masks.erase(rank.input_deduced_masks.begin() + row_i_lower);
                            rank.outputs.erase(rank.outputs.begin() + row_i_lower);
                            rank.output_done_masks.erase(rank.output_done_masks.begin() + row_i_lower);
                            rank.output_dc_masks.erase(rank.output_dc_masks.begin() + row_i_lower);
                            row_i_lower--; // compensate for deleted row, otherwise the row after the deleted one would be skipped
                        }
                    }
                }
            }
        }

        return all_prime_rows;
    }
}
