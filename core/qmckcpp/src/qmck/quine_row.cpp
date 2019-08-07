#include <qmck/quine_row.hpp>

qmck::quine_row::quine_row(qmck::logic_row &&lrow) noexcept
{
    this->inputs = lrow.inputs;
    this->outputs = lrow.outputs;
    this->outputs_dc_mask = lrow.outputs_dc_mask;
}

qmck::quine_row::quine_row(const qmck::logic_row &lrow)
{
    this->inputs = lrow.inputs;
    this->outputs = lrow.outputs;
    this->outputs_dc_mask = lrow.outputs_dc_mask;
}

std::vector<qmck::logic_value> qmck::quine_row::calculate_minterms_covered() const
{
    std::vector<qmck::logic_value> result{};
    result.push_back(inputs & ~inputs_deduced_mask); // value with all deduced bits as 0

    for (size_t i = 0; i < sizeof(qmck::logic_value) * 8; ++i)
    {
        if (((inputs_deduced_mask >> i) & 1u) == 1)
        {
            logic_value diff = 1u << i;

            size_t result_size = result.size();
            for (size_t j = 0; j < result_size; ++j){
                result.push_back(result[j] + diff);
            }
        }
    }

    return result;
}
