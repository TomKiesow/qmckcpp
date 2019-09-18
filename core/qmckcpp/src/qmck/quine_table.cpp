#include <qmck/quine_table.hpp>

qmck::quine_table::quine_table(const logic_table &lhs)
{
    this->format = lhs.format;

    for (auto &lrow : lhs.rows)
    {
        if (lrow.outputs)
        {
            quine_row qrow{lrow};

            int rank_i = __builtin_popcount(qrow.inputs);
            this->ranks[rank_i].push_back(qrow);
        }
    }
}

qmck::quine_table::quine_table(const generic_table_format &format)
{
    this->format = format;
}

bool qmck::quine_table::empty() const
{
    for (auto &rank : ranks)
    {
        if (!rank.empty())
        {
            return false;
        }
    }
    return true;
}
