#include <qmck/quine_table.hpp>

qmck::quine_table::quine_table(const logic_table &lhs)
{
    this->format = lhs.format;

    for (auto &lrow : lhs.rows)
    {
        if (lrow.outputs)
        {
            auto qrow = quine_row(lrow);

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

std::size_t qmck::quine_table::calculate_comparison_count_max()
{
    std::size_t count = 0;
    for (std::size_t i = 0; i < ranks.size() - 1; ++i)
    {
        auto& rank1 = ranks[i];
        auto& rank2 = ranks[i + 1];

        count += rank1.size() * rank2.size();
    }
    return count;
}
