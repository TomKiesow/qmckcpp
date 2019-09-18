#include <qmck/quine_row.hpp>

qmck::quine_row::quine_row(const logic_row &lrow) : basic_row()
{
    this->inputs = lrow.inputs;
    this->inputs_deduced_mask = 0;
    this->outputs = lrow.outputs;
    this->outputs_dc_mask = lrow.outputs_dc_mask;
}
