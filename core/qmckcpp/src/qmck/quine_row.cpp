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