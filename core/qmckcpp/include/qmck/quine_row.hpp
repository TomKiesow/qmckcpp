#pragma once

#include <qmck/basic_row.hpp>
#include <qmck/logic_value.hpp>
#include <qmck/logic_row.hpp>

#include <vector>

namespace qmck
{
    struct quine_row : basic_row
    {
    public:
        logic_value outputs_dc_mask;
        logic_value outputs_done_mask;

    public:
        quine_row() = default;

        explicit quine_row(const logic_row &lrow);
    };
}
