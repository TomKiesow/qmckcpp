#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace qmck
{
    class table_row
    {

    private:
        std::vector<bool> row;

    public:
        bool operator[](size_t index) // TODO
        {

            return row[index];
        }

        uint32_t getValues() // TODO
        {

            return 0;
        }

        bool getValueMask() // TODO
        {

            return false;
        }

        bool isCovered() // TODO
        {

            return false;
        }

        std::string str() // TODO
        {

            return std::string{};
        }

        uint32_t getFunctions() // TODO
        {

            return 0;
        }

        std::vector<int> getIndices() // TODO
        {

            return std::vector<int>{};
        }
    };

    bool mergeMakesSense(qmck::table_row r0, qmck::table_row r1) // TODO
    {

        return true;
    }

    qmck::table_row mergeRows(qmck::table_row r0, qmck::table_row r1, uint32_t i) // TODO
    {

        return qmck::table_row{};
    }
}
