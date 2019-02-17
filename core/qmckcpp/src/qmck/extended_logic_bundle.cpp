#include <qmck/extended_logic_bundle.hpp>

qmck::logic_bundle &qmck::logic_bundle::operator=(qmck::logic_bundle lhs)
{
    swap(lhs);
    return *this;
}

void qmck::logic_bundle::swap(qmck::logic_bundle &lhs)
{
    std::swap(inputs, lhs.inputs);
    std::swap(input_deduced_masks, lhs.input_deduced_masks);
    std::swap(outputs, lhs.outputs);
    std::swap(output_done_masks, lhs.output_done_masks);
    std::swap(output_dc_masks, lhs.output_dc_masks);
}
