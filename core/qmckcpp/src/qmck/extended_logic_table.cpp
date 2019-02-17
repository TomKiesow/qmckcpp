#include <qmck/extended_logic_table.hpp>

#include <bitset>

qmck::extended_logic_table::extended_logic_table(qmck::logic_table const &lhs)
{
    auto n = lhs.inputs.size();
    for (size_t i = 0; i < n; i++)
    {
        if (__builtin_popcount(lhs.outputs[i])) // ignore zero output
        {
            uint8_t rank = __builtin_popcount(lhs.inputs[i]);
            auto &it = logic_bundle_ranks.at(rank);

            it.inputs.push_back(lhs.inputs[i]);
            it.input_deduced_masks.push_back(uint32_t{0});
            it.outputs.push_back(lhs.outputs[i]);
            it.output_dc_masks.push_back(lhs.output_dc_masks[i]);
            it.output_done_masks.push_back((lhs.outputs[i] ^ UINT32_MAX) | lhs.output_dc_masks[i]);
        }
    }
}

qmck::extended_logic_table::extended_logic_table(qmck::logic_table &&lhs)
{
    auto n = lhs.inputs.size();
    for (size_t i = 0; i < n; i++)
    {
        if (__builtin_popcount(lhs.outputs[i])) // ignore zero output
        {
            uint8_t rank = __builtin_popcount(lhs.inputs[i]);
            auto it = logic_bundle_ranks[rank];

            it.inputs.push_back(lhs.inputs[i]);
            it.input_deduced_masks.push_back(uint32_t{0});
            it.outputs.push_back(lhs.outputs[i]);
            it.output_dc_masks.push_back(lhs.output_dc_masks[i]);
            it.output_done_masks.push_back((lhs.outputs[i] ^ UINT32_MAX) | lhs.output_dc_masks[i]);
        }
    }
}

std::string qmck::extended_logic_table::to_string()
{
    std::string result{};

    result += "inputs = ";
    result += std::to_string(format.inputs_count);
    result += "\noutputs = ";
    result += std::to_string(format.outputs_count);
    result += "\n";

    for (auto const &it : logic_bundle_ranks)
    {
        if (!it.inputs.empty())
        {
            size_t the_end = it.inputs.size();
            for (size_t the_idx{0}; the_idx < the_end; the_idx++)
            {
                std::bitset<32> input{it.inputs[the_idx]};
                std::bitset<32> input_deduced_mask{it.input_deduced_masks[the_idx]};
                std::bitset<32> output{it.outputs[the_idx]};
                std::bitset<32> output_dc_mask{it.output_dc_masks[the_idx]};
                std::bitset<32> output_done_mask{it.output_done_masks[the_idx]};

                result += input.to_string() + " | " + input_deduced_mask.to_string() + " | " + output.to_string() + " | " + output_dc_mask.to_string() + " | " + output_done_mask.to_string() + '\n';
            }
            result += '\n';
        }
    }

    return result;
}

qmck::extended_logic_table &qmck::extended_logic_table::operator=(qmck::extended_logic_table lhs)
{
    swap(lhs);
    return *this;
}

void qmck::extended_logic_table::swap(qmck::extended_logic_table &lhs)
{
    std::swap(format, lhs.format);
    std::swap(logic_bundle_ranks, lhs.logic_bundle_ranks);
}
