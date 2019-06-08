#include <utility>

#include <qmck/logic_table.hpp>
#include <qmck/logic_row.hpp>

#include <bitset>
#include <regex>

namespace
{
    enum symbol : char
    {
        DELIMINATOR = '|',
        ROW_DELIMINATOR = ';',
        HIGH = '1',
        LOW = '0',
        DONT_CARE = 'x'
    };

    inline char const *put_logic_table_format(qmck::generic_table_format &format, char const *table_c_str_begin, char const *table_c_str_end)
    {
        static std::regex const inputs_regex{R"(\s*inputs\s*=\s*([[:digit:]]+))"};
        static std::regex const outputs_regex{R"(\s*outputs\s*=\s*([[:digit:]]+))"};

        char const *it = table_c_str_begin;

        std::cmatch match{};
        if (std::regex_search(it, match, inputs_regex))
        {
            format.inputs_count = static_cast<uint32_t>(std::stoul(match.str(1)));
            it = match.suffix().first;
        }
        else
        {
            return table_c_str_end;
        }

        if (std::regex_search(it, match, outputs_regex))
        {
            format.outputs_count = static_cast<uint32_t>(std::stoul(match.str(1)));
            it = match.suffix().first;
        }
        else
        {
            return table_c_str_end;
        }

        return it;
    }

    inline char const *put_inputs(std::vector<qmck::logic_row> &rows, char const *table_c_str_begin, char const *table_c_str_end)
    {
        rows.push_back(qmck::logic_row{});

        char const *it = table_c_str_begin;
        for (; it < table_c_str_end; ++it)
        {
            switch (*it)
            {
            case symbol::HIGH:
            {
                for (auto &row:rows)
                {
                    row.inputs <<= 1u;
                    row.inputs |= 1u;
                }

                break;
            }
            case symbol::LOW:
            {
                for (auto &row:rows)
                {
                    row.inputs <<= 1u;
                }

                break;
            }
            case symbol::DONT_CARE:
            {
                size_t row_count = rows.size();
                for (size_t row_idx{0u}; row_idx < row_count; ++row_idx)
                {
                    rows[row_idx].inputs <<= 1u;

                    qmck::logic_row new_row{};
                    new_row.inputs = rows[row_idx].inputs | uint32_t{1u};
                    rows.push_back(new_row);
                }

                break;
            }
            case symbol::DELIMINATOR:
            {
                return ++it;
            }
            default:
            {
                break;
            }
            }
        }

        return it;
    }

    inline char const *put_outputs(std::vector<qmck::logic_row> &rows, char const *table_c_str_begin, char const *table_c_str_end)
    {
        char const *it = table_c_str_begin;
        for (; it < table_c_str_end; ++it)
        {
            switch (*it)
            {
            case symbol::HIGH:
            {
                size_t row_count = rows.size();
                for (size_t row_idx{0}; row_idx < row_count; ++row_idx)
                {
                    rows[row_idx].outputs <<= 1u;
                    rows[row_idx].outputs |= uint32_t{1u};

                    rows[row_idx].outputs_dc_mask <<= 1u;
                }

                break;
            }
            case symbol::LOW:
            {
                size_t row_count = rows.size();
                for (size_t row_idx{0}; row_idx < row_count; ++row_idx)
                {
                    rows[row_idx].outputs <<= 1u;
                    rows[row_idx].outputs_dc_mask <<= 1u;
                }

                break;
            }
            case symbol::DONT_CARE:
            {
                size_t row_count = rows.size();
                for (size_t row_idx{0}; row_idx < row_count; ++row_idx)
                {
                    rows[row_idx].outputs <<= 1u;
                    rows[row_idx].outputs |= uint32_t{1u};

                    rows[row_idx].outputs_dc_mask <<= 1u;
                    rows[row_idx].outputs_dc_mask |= uint32_t{1u};
                }

                break;
            }
            case symbol::ROW_DELIMINATOR:
            {
                return ++it;
            }
            default:
            {
                break;
            }
            }
        }

        return it;
    }
}

qmck::logic_table &qmck::logic_table::operator=(qmck::logic_table lhs)
{
    swap(*this, lhs);
    return *this;
}

void qmck::swap(qmck::logic_table &first, qmck::logic_table &second)
{
    using std::swap;
    swap(first.format, second.format);
    swap(first.rows, second.rows);
}

qmck::logic_table qmck::parse_logic_table(char const *i, char const *n)
{
    qmck::logic_table table{};
    i = put_logic_table_format(table.format, i, n);

    while (i < n)
    {
        std::vector<qmck::logic_row> rows_new;

        i = put_inputs(rows_new, i, n);
        i = put_outputs(rows_new, i, n);

        table.rows.insert(table.rows.end(), rows_new.begin(), rows_new.end());
    }

    return table;
}

// simple selection-sort
void qmck::logic_table::sort()
{
    int i_border = 0;

    do
    {
        int i_min = i_border;
        for (int i = i_border; i < rows.size(); ++i)
        {
            if (rows[i].inputs < rows[i_min].inputs)
            {
                i_min = i;
            }
        }

        qmck::logic_row tmp = rows[i_border];
        rows[i_border] = rows[i_min];
        rows[i_min] = tmp;
    }
    while (i_border++ < rows.size());
}
