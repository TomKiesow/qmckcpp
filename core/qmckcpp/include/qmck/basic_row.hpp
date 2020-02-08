#pragma once

#include <qmck/logic_value.hpp>
#include <qmck/generic_table_format.hpp>

#include <vector>

namespace qmck
{
    struct basic_row
    {
    public:
        logic_value inputs;
        logic_value inputs_deduced_mask;
        logic_value outputs;

    public:
        basic_row() = default;

        basic_row(const basic_row &other);

    public:
        std::vector<logic_value> calculate_minterms_covered() const;

    };
}