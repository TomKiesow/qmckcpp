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

        logic_value outputs_dc_mask{};
        logic_value outputs_done_mask{};

    public:

        constexpr quine_row() noexcept = default;

        constexpr quine_row(const quine_row &) noexcept = default;

        constexpr quine_row(quine_row &&) noexcept = default;

        constexpr explicit quine_row(const logic_row &lrow) noexcept: basic_row()
        {
            inputs = lrow.inputs;
            inputs_deduced_mask = 0;
            outputs = lrow.outputs;
            outputs_dc_mask = lrow.outputs_dc_mask;
        }

    public:

        constexpr quine_row &operator=(const quine_row &) noexcept = default;

        constexpr quine_row &operator=(quine_row &&) noexcept = default;
    };
}
