#pragma once

#include <qmck/table.hpp>
#include <qmck/table_format.hpp>
#include <vector>
#include <map>

#include <cstdlib>
#include <cstdint>

#include <iostream>
#include <cmath>
#include <algorithm>

namespace
{
    constexpr auto the_log2 = [](auto T) // TODO
    {
        return std::log2(T);
    };
}

namespace qmck
{
    void qmck(qmck::table original)
    {

        std::vector<qmck::table> tables{};

        auto table_format = original.get_table_format();
        auto current_order = qmck::table{original};
        auto next_order = current_order.get_empty_copy();

        std::vector<uint32_t> group_indices{}; // TODO right uint32_t type?
        auto merged_rows_last_iteration = true;
        uint32_t order_counter{}; // TODO right uint32_t type?

        while (merged_rows_last_iteration)
        {
            std::cout << "constructing table of order " << order_counter;
            merged_rows_last_iteration = false;
            current_order.prepare();
            group_indices = current_order.get_group_indices(); // must be called after prepare so unneded rows are filtered already

            size_t lastIndex{};
            size_t comparisonsDoneYet{};
            size_t comparisonsNeededTotal{};
            for (uint32_t i = 1; i < group_indices.size(); i++)
            {
                bool indexExceedsSize = (i + 1) >= group_indices.size();
                size_t current = group_indices[i] - lastIndex;
                auto next = (indexExceedsSize ? current_order.getRows()
                                                             .size() : group_indices[i + 1]) - group_indices[i];
                comparisonsNeededTotal += current * next;

                lastIndex = group_indices[i];
            }

            std::cout << "this table has " << group_indices.size() << " groups, " << current_order.getRows()
                                                                                                  .size() << " rows and will need " << comparisonsNeededTotal << " comparisons";

            for (uint32_t i = 0; i < group_indices.size() - 1; i++)
            {
                for (uint32_t x = group_indices[i]; x < group_indices[i + 1]; x++)
                {   // for each group index and its next
                    bool indexExceedsSize = (i + 2) >= group_indices.size();

                    for (uint32_t y = group_indices[i + 1]; y < (indexExceedsSize ? current_order.getRows()
                                                                                                 .size() : group_indices[i + 2]); y++)
                    {
                        auto rowX = current_order.getRows()[x];
                        auto rowY = current_order.getRows()[y];

                        comparisonsDoneYet++;
                        std::cout << "\r" << ((float) comparisonsDoneYet / comparisonsNeededTotal * 100) << "%";

                        if (!qmck::mergeMakesSense(rowX, rowY))
                        {
                            continue;
                        }

                        uint32_t rowXandrowY = (rowX.getValues() ^ rowY.getValues());
                        bool onlyOneDiff = (rowXandrowY & (rowXandrowY - 1)) == 0 && rowX.getValueMask() == rowY.getValueMask();
                        uint32_t diffIndex = static_cast<uint32_t> (the_log2(rowXandrowY));

                        if (onlyOneDiff)
                        {
                            merged_rows_last_iteration = true;
                            qmck::table_row newRow = qmck::mergeRows(rowX, rowY, diffIndex);
                            next_order.addRow(newRow);
                        }
                    }
                }
            }

            tables.push_back(current_order);
            current_order = next_order;
            next_order = current_order.get_empty_copy();
            std::cout << '\n';
            std::cout << "done constructing table of order " << order_counter++;
        }

        std::cout << "done with the tables... calculating coverage";

        std::map<char, qmck::table_row> uncoveredRows{};
        char current_char = 'A';

        for (uint32_t i = 0; i < tables.size(); i++)
        {
            for (auto row : tables[i].getRows())
            {
                if (!row.isCovered())
                {
                    uncoveredRows[current_char++] = row;
                }
            }
        }

        uint32_t colCountRight = table_format.getColCountRight();
        uint32_t colCountLeft = table_format.getColCountLeft();
        uint32_t termCount = 1u << colCountLeft;
        std::string coverageTable[colCountRight][termCount];

        for (uint32_t i = 0; i < colCountRight; i++)
        {
            for (uint32_t j = 0; j < termCount; j++)
            {
                coverageTable[i][j] = "";
            }
        }

        std::cout << "all uncovered rows:";
        for (const auto &termId : uncoveredRows)
        {
            qmck::table_row current_row = termId.second;
            std::cout << termId.first << "   " << current_row.str();

            uint32_t functions = current_row.getFunctions();
            for (uint32_t f = 0; f < colCountRight; f++)
            {
                if (((functions >> (32 - f)) & 1) == 1)
                {
                    for (uint32_t i = 0; i < termCount; i++)
                    {
                        if (std::find(current_row.getIndices()
                                                 .begin(), current_row.getIndices()
                                                                      .end(), i) != current_row.getIndices()
                                                                                               .end())
                        {
                            coverageTable[f][i] += termId.first;
                        }
                    }
                }
            }
        }

        std::cout << '\n';
        for (uint32_t i = 0; i < tables.size(); i++)
        {
            std::cout << "table of order " << i;
            std::cout << tables[i].str();
        }

        std::string sb{};
        for (uint32_t f = 0; f < colCountRight; f++)
        {
            sb += (char) f;
            sb += ": ";
            for (uint32_t i = 0; i < termCount; i++)
            {
                if (coverageTable[f][i].length() != 0)
                {
                    sb += ('(');
                    for (uint32_t j = 0; j < coverageTable[f][i].length(); j++)
                    {
                        sb += (coverageTable[f][i][j]);
                        if (j != coverageTable[f][i].length() - 1)
                        {
                            sb.append(" + ");
                        }
                    }
                    sb += (')');
                    if (i != termCount - 1)
                    {
                        sb.append(" * ");
                    }
                }
            }
            sb += '\n';
        }

        std::cout << '\n';
        std::cout << sb;
    }
}
