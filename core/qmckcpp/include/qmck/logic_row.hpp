#pragma once

#include <qmck/logic_value.hpp>

namespace qmck
{
    struct logic_row
    {
    public:

        logic_value inputs;
        logic_value outputs_dc_mask;
        logic_value outputs;

    public:

        constexpr logic_row() noexcept = default;

        constexpr logic_row(const logic_row &) noexcept = default;

        constexpr logic_row(logic_row &&) noexcept = default;

    public:

        constexpr logic_row &operator=(const logic_row &) noexcept = default;

        constexpr logic_row &operator=(logic_row &&) noexcept = default;
    };
}
