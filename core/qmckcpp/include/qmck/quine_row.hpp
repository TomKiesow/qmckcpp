#pragma once

#include <qmck/logic_value.hpp>
#include <qmck/logic_row.hpp>
#include <qmck/generic_table_format.hpp>

namespace qmck
{
    struct quine_row
    {
    public:
        logic_value inputs{};
        logic_value inputs_deduced_mask{};
        logic_value outputs{};
        logic_value outputs_dc_mask{};
        logic_value outputs_done_mask{};

    public:
        quine_row() = default;
        explicit quine_row(logic_row const &lrow);
        explicit quine_row(logic_row &&lrow) noexcept;
    };
}
