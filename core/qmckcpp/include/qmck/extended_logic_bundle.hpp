#pragma once

#include <qmck/logic_array.hpp>

namespace qmck
{
    struct logic_bundle
    {
    public:
        logic_array inputs{};
        logic_array input_deduced_masks{};
        logic_array outputs{};
        logic_array output_done_masks{};
        logic_array output_dc_masks{};

    public:

        logic_bundle() = default;

        logic_bundle(logic_bundle const &lhs) = default;

        logic_bundle(logic_bundle &&lhs) noexcept = default;

        logic_bundle &operator=(logic_bundle lhs);

        void swap(logic_bundle &lhs);
    };
}
