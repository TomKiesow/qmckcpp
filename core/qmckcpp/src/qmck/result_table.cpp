#include <qmck/result_table.hpp>

qmck::result_table::result_table(const quine_table &lhs)
{
    this->format = lhs.format;

    for(std::size_t rank_i = 0; rank_i < lhs.ranks.size(); ++rank_i){
        auto &rank = lhs.ranks[rank_i];

        for(std::size_t row_i = 0; row_i < rank.size(); ++row_i){
            auto &row = rank[row_i];

            add_row(result_row(row, format));
        }
    }
}

void qmck::result_table::add_row(result_row &&row)
{
    rows[rows.size()] = row;
}
