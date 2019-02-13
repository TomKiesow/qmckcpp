#pragma once

#include <qmck/table_format.hpp>
#include <qmck/table_row.hpp>
#include <vector>
#include <cstdint>

namespace qmck
{
    class table
    {

    public:
        qmck::table_format get_table_format() // TODO
        {

            return qmck::table_format{};
        }

        qmck::table get_empty_copy()// TODO
        {

            return table{};
        }

        std::vector<uint32_t> get_group_indices()// TODO
        {

            return std::vector<uint32_t>{};
        }

        void prepare()// TODO
        {

        }

        std::vector<qmck::table_row> getRows()// TODO
        {

        }

        void addRow(qmck::table_row row)// TODO
        {

        }

        std::string str()// TODO
        {

            return std::string{};
        }
    };
}
