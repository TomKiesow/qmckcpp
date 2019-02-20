#include <qmck/extended_logic_bundle.hpp>

qmck::extended_logic_bundle &qmck::extended_logic_bundle::operator=(qmck::extended_logic_bundle lhs)
{
    swap(*this, lhs);
    return *this;
}

void qmck::swap(qmck::extended_logic_bundle &first, qmck::extended_logic_bundle &second)
{
    using std::swap;
    swap(first.inputs, second.inputs);
    swap(first.input_deduced_masks, second.input_deduced_masks);
    swap(first.outputs, second.outputs);
    swap(first.output_dc_masks, second.output_dc_masks);
    swap(first.output_done_masks, second.output_done_masks);
}
