#include <qmck/result_row.hpp>

// object slicing happening here
// dunno how to fix that, but its works like this
qmck::result_row::result_row(const quine_row &lrow, const generic_table_format &format) : basic_row(lrow)
{
    this->outputs = lrow.outputs;

    cost = calculate_cost(format);
}

int qmck::result_row::calculate_cost(const generic_table_format &format) const
{
    int cost{0};
    for (std::uint32_t j{0}; j < format.inputs_count; ++j)
    {
        if (!((inputs_deduced_mask >> (format.inputs_count - j - 1)) & 1u))
        {
            ++cost;
        }
    }
    return cost;
}
