#include <utility>

#include <qmck/logic_table.hpp>
#include <qmck/logic_row.hpp>
#include <qmck/logic_value.hpp>

#include <regex>
#include <string>
#include <iostream>

namespace
{
    enum symbol : char
    {
        DELIMINATOR = '|',
        ROW_DELIMINATOR = ';',
        HIGH = '1',
        LOW = '0',
        DONT_CARE = 'x',
        SPACE = ' ',
        TAB = '\t',
        CR = '\r',
        LF = '\n'
    };

    char const *advance_until(char const *it, char end, int &line_counter)
    {
        while (*it != end)
        {
            if (*it == '\n')
            {
                ++line_counter;
            }
            ++it;
        }
        return ++it;
    }

    inline char const *put_logic_table_format(qmck::generic_table_format &format, char const *table_c_str_begin, char const *table_c_str_end, int &line_counter)
    {
        // include all linefeeds from beginning to start of table in the regexes
        // and count them in the matches to keep line_counter accurate
        static std::regex const inputs_regex{R"(^(\s*)inputs[ \t]*=[ \t]*([[:digit:]]+))"};
        static std::regex const outputs_regex{R"(^(\s*)outputs[ \t]*=[ \t]*([[:digit:]]+)(\s*))"};

        char const *it = table_c_str_begin;

        std::cmatch match{};
        if (std::regex_search(it, match, inputs_regex))
        {
            format.inputs_count = static_cast<uint32_t>(std::stoul(match.str(2)));
            it = match.suffix().first;

            // count linefeeds from beginning of file to 'inputs=...'
            auto match_str = match.str(1);
            line_counter += std::count(match_str.begin(), match_str.end(), '\n');
        }
        else
        {
            std::cout << "line " << line_counter << " of input file: expected 'inputs=n'\n";
            return table_c_str_end;
        }

        if (std::regex_search(it, match, outputs_regex))
        {
            format.outputs_count = static_cast<uint32_t>(std::stoul(match.str(2)));
            it = match.suffix().first;

            // count linefeeds from end of 'inputs=...' to 'outputs=...'
            auto match_str1 = match.str(1);
            line_counter += std::count(match_str1.begin(), match_str1.end(), '\n');

            // count linefeeds from end of 'outputs=...' to start of first row or end of file
            auto match_str2 = match.str(3);
            line_counter += std::count(match_str2.begin(), match_str2.end(), '\n');
        }
        else
        {
            std::cout << "line " << line_counter << " of input file: expected 'outputs=n'\n";
            return table_c_str_end;
        }

        return it;
    }

    inline char const *put_inputs(std::vector<qmck::logic_row> &rows, char const *table_c_str_begin, char const *table_c_str_end, qmck::generic_table_format &format, int &line_counter)
    {
        rows.push_back(qmck::logic_row{});

        auto inputs_todo_count = format.inputs_count;
        char const *it = table_c_str_begin;
        for (; it < table_c_str_end; ++it)
        {
            if (inputs_todo_count == 0)
            {
                switch (*it)
                {
                case symbol::HIGH:
                case symbol::LOW:
                case symbol::DONT_CARE:
                    std::cout << "line " << line_counter << " of input file: row malformed: expected '" << symbol::DELIMINATOR << "', found '" << *it << "'. discarding surrounding row\n";
                    rows.clear();
                    return advance_until(it, symbol::DELIMINATOR, line_counter);
                }
            }

            switch (*it)
            {
            case symbol::HIGH:
            {
                --inputs_todo_count;
                for (auto &row:rows)
                {
                    row.inputs <<= 1u;
                    row.inputs |= 1u;
                }

                break;
            }
            case symbol::LOW:
            {
                --inputs_todo_count;
                for (auto &row:rows)
                {
                    row.inputs <<= 1u;
                }

                break;
            }
            case symbol::DONT_CARE:
            {
                --inputs_todo_count;
                std::size_t row_count = rows.size();
                for (std::size_t row_idx{0u}; row_idx < row_count; ++row_idx)
                {
                    rows[row_idx].inputs <<= 1u;

                    qmck::logic_row new_row{};
                    new_row.inputs = rows[row_idx].inputs | qmck::logic_value{1u};
                    rows.push_back(new_row);
                }

                break;
            }
            case symbol::DELIMINATOR:
            {
                return ++it;
            }
                // silently consume those
            case symbol::LF:
                ++line_counter;
            case symbol::SPACE:
            case symbol::TAB:
            case symbol::CR:
                break;
            default:
            {
                if (inputs_todo_count == 0)
                {
                    std::cout << "line " << line_counter << " of input file: row malformed: expected '" << symbol::DELIMINATOR << "', found '" << *it << "'. discarding surrounding row\n";
                }
                else
                {
                    std::cout << "line " << line_counter << " of input file: row malformed: found '" << *it << "'. discarding surrounding row\n";
                }

                rows.clear();
                return advance_until(it, symbol::DELIMINATOR, line_counter);
            }
            }
        }

        if (inputs_todo_count == 0)
        {
            std::cout << "reached end of input file: row malformed: expected '" << symbol::DELIMINATOR << "', discarding row\n";
        }
        else
        {
            if (inputs_todo_count < format.inputs_count)
            {
                // in this case we have already read a {0,1,x} of a new row
                std::cout << "reached end of input file: row malformed: row unfinished, discarding row\n";
            }
        }

        rows.clear();

        return it;
    }

    inline char const *put_outputs(std::vector<qmck::logic_row> &rows, char const *table_c_str_begin, char const *table_c_str_end, qmck::generic_table_format &format, int &line_counter)
    {
        auto outputs_todo_count = format.outputs_count;
        char const *it = table_c_str_begin;
        for (; it < table_c_str_end; ++it)
        {
            if (outputs_todo_count == 0)
            {
                switch (*it)
                {
                case symbol::HIGH:
                case symbol::LOW:
                case symbol::DONT_CARE:
                    std::cout << "line " << line_counter << " of input file: row malformed: expected '" << symbol::ROW_DELIMINATOR << "', found '" << *it << "'. discarding surrounding row\n";
                    rows.clear();
                    return advance_until(it, symbol::ROW_DELIMINATOR, line_counter);
                }
            }

            switch (*it)
            {
            case symbol::HIGH:
            {
                --outputs_todo_count;
                std::size_t row_count = rows.size();
                for (std::size_t row_idx{0}; row_idx < row_count; ++row_idx)
                {
                    rows[row_idx].outputs <<= 1u;
                    rows[row_idx].outputs |= qmck::logic_value{1u};

                    rows[row_idx].outputs_dc_mask <<= 1u;
                }

                break;
            }
            case symbol::LOW:
            {
                --outputs_todo_count;
                std::size_t row_count = rows.size();
                for (std::size_t row_idx{0}; row_idx < row_count; ++row_idx)
                {
                    rows[row_idx].outputs <<= 1u;
                    rows[row_idx].outputs_dc_mask <<= 1u;
                }

                break;
            }
            case symbol::DONT_CARE:
            {
                --outputs_todo_count;
                std::size_t row_count = rows.size();
                for (std::size_t row_idx{0}; row_idx < row_count; ++row_idx)
                {
                    rows[row_idx].outputs <<= 1u;
                    rows[row_idx].outputs |= qmck::logic_value{1u};

                    rows[row_idx].outputs_dc_mask <<= 1u;
                    rows[row_idx].outputs_dc_mask |= qmck::logic_value{1u};
                }

                break;
            }
            case symbol::ROW_DELIMINATOR:
            {
                return ++it;
            }
                // silently consume those
            case symbol::LF:
                ++line_counter;
            case symbol::SPACE:
            case symbol::TAB:
            case symbol::CR:
                break;
            default:
            {
                if (outputs_todo_count == 0)
                {
                    std::cout << "line " << line_counter << " of input file: row malformed: expected '" << symbol::ROW_DELIMINATOR << "', found '" << *it << "'. discarding surrounding row\n";
                }
                else
                {
                    std::cout << "line " << line_counter << " of input file: row malformed: found '" << *it << "'. discarding surrounding row\n";
                }

                rows.clear();
                return advance_until(it, symbol::ROW_DELIMINATOR, line_counter);
            }
            }
        }

        if (outputs_todo_count == 0)
        {
            std::cout << "reached end of input file: row malformed: expected '" << symbol::ROW_DELIMINATOR << "', discarding row\n";
        }
        else
        {
            std::cout << "reached end of input file: row malformed: row unfinished, discarding row\n";
        }

        rows.clear();

        return it;
    }
}

qmck::logic_table &qmck::logic_table::operator=(logic_table lhs)
{
    swap(*this, lhs);
    return *this;
}

void qmck::swap(logic_table &first, logic_table &second)
{
    using std::swap;
    swap(first.format, second.format);
    swap(first.rows, second.rows);
}

qmck::logic_table qmck::parse_logic_table(char const *i, char const *n)
{
    int line_counter = 1; // to give useful error information
    qmck::logic_table table{};
    i = put_logic_table_format(table.format, i, n, line_counter);

    while (i < n)
    {
        std::vector<qmck::logic_row> rows_new;

        i = put_inputs(rows_new, i, n, table.format, line_counter);
        // dont need to run put_outputs if row is getting discarded anyway
        if (!rows_new.empty())
        {
            i = put_outputs(rows_new, i, n, table.format, line_counter);
        }

        table.rows.insert(table.rows.end(), rows_new.begin(), rows_new.end());
    }

    return table;
}
