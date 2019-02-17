#include <qmck/logic_table_format.hpp>

#include <utility>

qmck::logic_table_format::logic_table_format(uint32_t inputs_count, uint32_t outputs_count) : inputs_count(inputs_count), outputs_count(outputs_count)
{
}

qmck::logic_table_format &qmck::logic_table_format::operator=(qmck::logic_table_format lhs) noexcept
{
    swap(lhs);
    return *this;
}

void qmck::logic_table_format::swap(qmck::logic_table_format &lhs)
{
    std::swap(inputs_count, lhs.inputs_count);
    std::swap(outputs_count, lhs.outputs_count);
}
