#include <qmck/generic_table_format.hpp>

#include <utility>

qmck::generic_table_format::generic_table_format(uint32_t inputs_count, uint32_t outputs_count) : inputs_count(inputs_count), outputs_count(outputs_count)
{
}

qmck::generic_table_format &qmck::generic_table_format::operator=(qmck::generic_table_format lhs)
{
    swap(*this, lhs);
    return *this;
}

void qmck::swap(qmck::generic_table_format &first, qmck::generic_table_format &second)
{
    using std::swap;
    swap(first.inputs_count, second.inputs_count);
    swap(first.outputs_count, second.outputs_count);
}
