#pragma once

#include <qmck/logic_value.hpp>
#include <qmck/generic_table_format.hpp>

namespace qmck
{
    struct logic_row
    {
    public:
        logic_value inputs{};
        logic_value outputs_dc_mask{};
        logic_value outputs{};
    };
}
