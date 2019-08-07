#pragma once

#include <qmck/logic_value.hpp>

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
