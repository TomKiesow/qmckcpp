#pragma once

#include <cstdint>

namespace qmck
{
    struct logic_table_format
    {
    public:
        uint32_t inputs_count{};
        uint32_t outputs_count{};

        logic_table_format(uint32_t inputs_count, uint32_t outputs_count);

        logic_table_format() = default;

        logic_table_format(logic_table_format const &lhs) = default;

        logic_table_format(logic_table_format &&rhs) = default;

        logic_table_format &operator=(logic_table_format lhs) noexcept;

        void swap(logic_table_format &lhs);
    };
}
