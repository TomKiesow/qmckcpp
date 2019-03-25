#include <qmck.hpp>

#include <iostream>
#include <list>

namespace qmck
{
    qmck::extended_logic_table deduce(qmck::logic_table table)
    {
        std::cout << table << std::endl;
        // custom : logic_table
        //
        // inputs | outputs
        // -------+--------
        // 00 00  |    01 1
        // 00 01  |    00 0
        // 00 10  |    00 0
        // 00 11  |    00 0
        //
        // 01 00  |    10 1
        // 01 01  |    01 0
        // 01 10  |    01 0
        // 01 11  |    01 0
        //
        // 10 0x  |    00 1
        // 10 1x  |    10 0
        //
        // 11 x0  |    00 x
        // 11 x1  |    11 x

        // TODO convert custom into 1. reduction_logic_table
        // 0. delete rows where output equals false
        // 1. blow up rows with dont cares in the input
        // 2. groups rows in order how many ones it contains
        // 3. create output_mask that is initialized as not output
        //    +-> tread dont cares in the output as zeros

        qmck::extended_logic_table extended_table{table};
        std::cout << extended_table << std::endl;

        // =>
        // custom : 1. reduction_logic_table
        //
        //  idx      1's | input | output | output_mask
        // -----     ----+-------+--------+------------
        //   0       0   | 00 00 |   01 1 |        10 0
        // -----     ----+-------+--------+------------
        //   4       1   | 01 00 |   10 1 |        01 0
        //   8           | 10 00 |   00 1 |        11 0
        // -----     ----+-------+--------+------------
        //   5       2   | 01 01 |   01 0 |        10 1
        //   6           | 01 10 |   01 0 |        10 1
        //   9           | 10 01 |   00 1 |        11 0
        //  10           | 10 10 |   10 0 |        01 1
        //  12           | 11 00 |   00 x |        11 1
        // -----     ----+-------+--------+------------
        //   7       3   | 01 11 |   01 0 |        10 1
        //  11           | 10 11 |   10 0 |        01 1
        //  13           | 11 01 |   11 x |        00 1
        //  14           | 11 10 |   00 x |        11 1
        // -----     ----+-------+--------+------------
        //  15       4   | 11 11 |   11 x |        00 1

        // TODO create 2. reduction_logic_table
        // 0. find and merge possible rows from 1. reduction_logic_table into 2. reduction_logic_table
        //    +-> modify the output mask from 1. reduction_logic_table for found merges

        std::list<extended_logic_table> all_extended_logic_tables;
        all_extended_logic_tables.push_back(extended_table);

        while (!all_extended_logic_tables.back().empty())
        {
            auto &extended_table_current = all_extended_logic_tables.back();

            all_extended_logic_tables.emplace_back(extended_logic_table{extended_table_current.format});
            auto &extended_table_next = all_extended_logic_tables.back();

            auto n = extended_table_current.logic_bundle_ranks.size() - 1;
            for (size_t i{0}; i < n; i++)
            {
                auto &next_bundle = extended_table_next.logic_bundle_ranks[i];
                auto &lower_bundle = extended_table_current.logic_bundle_ranks[i];
                auto &upper_bundle = extended_table_current.logic_bundle_ranks[i + 1];

                auto lower_n = lower_bundle.inputs.size();
                for (int lower_i{0}; lower_i < lower_n; lower_i++)
                {
                    auto upper_n = upper_bundle.inputs.size();
                    for (int upper_i{0}; upper_i < upper_n; upper_i++)
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
        }

        // remove empty one at the end
        all_extended_logic_tables.pop_back();

        int counter{0};
        for (auto &extended_logic_table:all_extended_logic_tables)
        {
            std::cout << "table of order " << counter++ << std::endl << extended_logic_table << std::endl;
        }

        // =>
        // custom : 1. reduction_logic_table
        //
        //  idx      1's | input | output | output_mask
        // -----     ----+-------+--------+------------
        //   0       0   | 00 00 |   01 1 |        10 1 <-
        // -----     ----+-------+--------+------------
        //   4       1   | 01 00 |   10 1 |        01 1 <-
        //   8           | 10 00 |   00 1 |        11 1
        // -----     ----+-------+--------+------------
        //   5       2   | 01 01 |   01 0 |        11 1
        //   6           | 01 10 |   01 0 |        11 1
        //   9           | 10 01 |   00 1 |        11 1
        //  10           | 10 10 |   10 0 |        11 1
        //  12           | 11 00 |   00 x |        11 1
        // -----     ----+-------+--------+------------
        //   7       3   | 01 11 |   01 0 |        11 1
        //  11           | 10 11 |   10 0 |        11 1
        //  13           | 11 01 |   11 x |        01 1 <-
        //  14           | 11 10 |   00 x |        11 1
        // -----     ----+-------+--------+------------
        //  15       4   | 11 11 |   11 x |        11 1

        // =>
        // custom : 2. reduction_logic_table
        //
        //   idx       1's | input | output | output_mask
        // -------     ----+-------+--------+------------
        //   0,4       0   | 0- 00 |   00 1 |        11 0
        //   0,8           | -0 00 |   00 1 |        11 0
        // -------     ----+-------+--------+------------
        //   4,5       1   | 01 0- |   00 0 |        11 1
        //   4,6           | 01 -0 |   00 0 |        11 1
        //   4,12          | -1 00 |   00 1 |        11 0
        //   8,9           | 10 0- |   00 1 |        11 0
        //   8,10          | 10 -0 |   00 0 |        11 1
        //   8,12          | 1- 00 |   00 1 |        11 0
        // -------     ----+-------+--------+------------
        //   5,7       2   | 01 -1 |   01 0 |        10 1
        //   5,13          | -1 01 |   01 0 |        10 1
        //   6,7           | 01 1- |   01 0 |        10 1
        //   6,14          | -1 10 |   00 0 |        11 1
        //   9,11          | 10 -1 |   00 0 |        11 1
        //   9,13          | 1- 01 |   00 1 |        11 0
        //  10,11          | 10 1- |   10 0 |        01 1
        //  10,14          | 1- 10 |   00 0 |        11 1
        //  12,13          | 11 0- |   00 x |        11 1
        //  12,14          | 11 -0 |   00 x |        11 1
        // -------     ----+-------+--------+------------
        //   7,15      3   | -1 11 |   01 0 |        10 1
        //  11,15          | 1- 11 |   10 0 |        01 1
        //  13,15          | 11 -1 |   11 x |        00 1
        //  14,15          | 11 1- |   00 x |        11 1

        // =>
        // custom : 2. reduction_logic_table
        //
        //   idx       1's | input | output | output_mask
        // -------     ----+-------+--------+------------
        //   0,4       0   | 0- 00 |   00 1 |        11 0
        //   0,8           | -0 00 |   00 1 |        11 0
        // -------     ----+-------+--------+------------
        //   4,12          | -1 00 |   00 1 |        11 0
        //   8,9           | 10 0- |   00 1 |        11 0
        //   8,12          | 1- 00 |   00 1 |        11 0
        // -------     ----+-------+--------+------------
        //   5,7       2   | 01 -1 |   01 0 |        10 1
        //   5,13          | -1 01 |   01 0 |        10 1
        //   6,7           | 01 1- |   01 0 |        10 1
        //   9,13          | 1- 01 |   00 1 |        11 0
        //  10,11          | 10 1- |   10 0 |        01 1
        //  12,13          | 11 0- |   00 x |        11 1
        //  12,14          | 11 -0 |   00 x |        11 1
        // -------     ----+-------+--------+------------
        //   7,15      3   | -1 11 |   01 0 |        10 1
        //  11,15          | 1- 11 |   10 0 |        01 1
        //  13,15          | 11 -1 |   11 x |        00 1
        //  14,15          | 11 1- |   00 x |        11 1

        // TODO create 3. - n. reduction_logic_table with a loop

        // =>
        // custom : 2. reduction_logic_table
        //
        //   idx       1's | input | output | output_mask
        // -------     ----+-------+--------+------------
        //   0,4       0   | 0- 00 |   00 1 |        11 1
        //   0,8           | -0 00 |   00 1 |        11 1
        // -------     ----+-------+--------+------------
        //   4,12          | -1 00 |   00 1 |        11 1
        //   8,9           | 10 0- |   00 1 |        11 1
        //   8,12          | 1- 00 |   00 1 |        11 1
        // -------     ----+-------+--------+------------
        //   5,7       2   | 01 -1 |   01 0 |        11 1
        //   5,13          | -1 01 |   01 0 |        11 1
        //   6,7           | 01 1- |   01 0 |        10 1 <-
        //   9,13          | 1- 01 |   00 1 |        11 1
        //  10,11          | 10 1- |   10 0 |        01 1 <-
        //  12,13          | 11 0- |   00 x |        11 1
        //  12,14          | 11 -0 |   00 x |        11 1
        // -------     ----+-------+--------+------------
        //   7,15      3   | -1 11 |   01 0 |        11 1
        //  11,15          | 1- 11 |   10 0 |        01 1 <-
        //  13,15          | 11 -1 |   11 x |        01 1 <-
        //  14,15          | 11 1- |   00 x |        11 1

        // =>
        // custom : 3. reduction_logic_table
        //
        //      idx          1's | input | output | output_mask
        // -------------     ----+-------+--------+------------
        //    0,4,8,12       0   | -- 00 |   00 1 |        11 0
        //    0,4,8,12           | -- 00 |   00 1 |        11 0
        // -------------     ----+-------+--------+------------
        //    8,9,10,11          | 10 -- |   00 1 |        11 0
        //    8,9,12,13          | 1- 0- |   00 1 |        11 0
        //   8,12,9,13           | 1- 0- |   00 1 |        11 0
        // -------------     ----+-------+--------+------------
        //    5,7,13,15      2   | -1 -1 |   01 0 |        10 1
        //   5,13,7,15           | -1 -1 |   01 0 |        10 1
        //  12,13,14,15          | 11 -- |   00 x |        11 1
        //  12,14,13,15          | 11 -- |   00 x |        11 1

        // =>
        // custom : 3. reduction_logic_table
        //
        //      idx          1's | input | output | output_mask
        // -------------     ----+-------+--------+------------
        //    0,4,8,12       0   | -- 00 |   00 1 |        11 0 <-
        // -------------     ----+-------+--------+------------
        //    8,9,10,11          | 10 -- |   00 1 |        11 1
        //    8,9,12,13          | 1- 0- |   00 1 |        11 0 <-
        // -------------     ----+-------+--------+------------
        //    5,7,13,15      2   | -1 -1 |   01 0 |        10 1 <-
        //  12,13,14,15          | 11 -- |   00 x |        11 1

        // =>
        // custom : 4. reduction_logic_table
        //
        //            idx                1's | input | output | output_mask
        // -------------------------     ----+-------+--------+------------
        //    8,9,10,11,12,13,14,15          | 1- -- |   00 1 |        11 0 <-

        return extended_table;
    }
}
