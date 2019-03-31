#include <qmck/extended_logic_table.hpp>

#include <bitset>

qmck::extended_logic_table::extended_logic_table(qmck::logic_table const &lhs)
{
    this->format = lhs.format;

    auto n = lhs.inputs.size();
    for (size_t i = 0; i < n; ++i)
    {
        if (lhs.outputs[i]) // ignore zero output
        {
            uint8_t rank = __builtin_popcount(lhs.inputs[i]);
            auto &it = logic_bundle_ranks.at(rank);

            it.inputs.push_back(lhs.inputs[i]);
            it.input_deduced_masks.push_back(uint32_t{0});
            it.outputs.push_back(lhs.outputs[i]);
            it.output_dc_masks.push_back(lhs.output_dc_masks[i]);
            it.output_done_masks.push_back(lhs.output_dc_masks[i]);
        }
    }
}

qmck::extended_logic_table::extended_logic_table(qmck::logic_table &&lhs)
{
    this->format = lhs.format;

    auto n = lhs.inputs.size();
    for (size_t i = 0; i < n; ++i)
    {
        if (lhs.outputs[i]) // ignore zero output
        {
            auto rank = static_cast<uint8_t>(__builtin_popcount(lhs.inputs[i]));
            auto it = logic_bundle_ranks[rank];

            it.inputs.push_back(lhs.inputs[i]);
            it.input_deduced_masks.push_back(uint32_t{0});
            it.outputs.push_back(lhs.outputs[i]);
            it.output_dc_masks.push_back(lhs.output_dc_masks[i]);
            it.output_done_masks.push_back(lhs.output_dc_masks[i]);
        }
    }
}

qmck::extended_logic_table::extended_logic_table(qmck::logic_table_format &format)
{
    this->format = format;
}

qmck::extended_logic_table &qmck::extended_logic_table::operator=(qmck::extended_logic_table lhs)
{
    swap(*this, lhs);
    return *this;
}

void qmck::swap(qmck::extended_logic_table &first, qmck::extended_logic_table &second)
{
    using std::swap;
    swap(first.format, second.format);
    swap(first.logic_bundle_ranks, second.logic_bundle_ranks);
}

bool qmck::extended_logic_table::empty() const
{
    for (auto const &bundle:logic_bundle_ranks)
    {
        if (!bundle.inputs.empty())
        {
            return false;
        }
    }
    return true;
}
