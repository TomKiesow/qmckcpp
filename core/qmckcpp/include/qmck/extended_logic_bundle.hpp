#pragma once

#include <qmck/logic_array.hpp>
#include <string>

namespace qmck
{
    struct extended_logic_bundle
    {
    public:
        logic_array inputs{};
        logic_array input_deduced_masks{};
        logic_array outputs{};
        logic_array output_done_masks{};
        logic_array output_dc_masks{};

    public:

        extended_logic_bundle() = default;

        extended_logic_bundle(extended_logic_bundle const &lhs) = default;

        extended_logic_bundle(extended_logic_bundle &&lhs) noexcept = default;

    public:

        extended_logic_bundle &operator=(extended_logic_bundle lhs);

        friend void swap(extended_logic_bundle &first, extended_logic_bundle &second);
    };
}
