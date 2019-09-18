#include <qmck/basic_row.hpp>

qmck::basic_row::basic_row(const qmck::basic_row &other)
{
    inputs = other.inputs;
    outputs = other.outputs;
    inputs_deduced_mask = other.inputs_deduced_mask;
}

std::vector<qmck::logic_value> qmck::basic_row::calculate_minterms_covered() const
{
    std::vector<qmck::logic_value> result{};
    result.push_back(inputs & ~inputs_deduced_mask); // value with all deduced bits as 0

    for (std::size_t i = 0; i < sizeof(qmck::logic_value) * 8; ++i)
    {
        if (((inputs_deduced_mask >> i) & 1u) == 1)
        {
            logic_value diff = 1u << i;

            std::size_t result_size = result.size();
            for (std::size_t j = 0; j < result_size; ++j){
                result.push_back(result[j] + diff);
            }
        }
    }

    return result;
}
