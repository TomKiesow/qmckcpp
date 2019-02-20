#include <qmck/logic_table_format.hpp>

#include <utility>

qmck::logic_table_format::logic_table_format(uint32_t inputs_count, uint32_t outputs_count) : inputs_count(inputs_count), outputs_count(outputs_count)
{
}

qmck::logic_table_format &qmck::logic_table_format::operator=(qmck::logic_table_format lhs)
{
    swap(*this, lhs);
    return *this;
}

void qmck::swap(qmck::logic_table_format &first, qmck::logic_table_format &second)
{
    using std::swap;
    swap(first.inputs_count, second.inputs_count);
    swap(first.outputs_count, second.outputs_count);
}
