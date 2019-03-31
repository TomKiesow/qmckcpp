#include <utility>

#include <qmck/logic_table.hpp>

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

    inline char const *put_logic_table_format(qmck::logic_table_format &format, char const *table_c_str_begin, char const *table_c_str_end)
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

    inline char const *put_inputs(qmck::logic_array &inputs, qmck::logic_array &outputs, qmck::logic_array &output_masks, char const *table_c_str_begin, char const *table_c_str_end)
    {
        char const *it = table_c_str_begin;
        for (; it < table_c_str_end; ++it)
        {
            switch (*it)
            {
            case symbol::HIGH:
            {
                for (auto &input:inputs)
                {
                    input <<= 1;
                    input |= 1;
                }

                break;
            }
            case symbol::LOW:
            {
                for (auto &input:inputs)
                {
                    input <<= 1;
                }

                break;
            }
            case symbol::DONT_CARE:
            {
                size_t input_end = inputs.size();
                for (size_t input_idx{0u}; input_idx < input_end; ++input_idx)
                {
                    inputs[input_idx] <<= 1;
                    inputs.push_back(inputs[input_idx] | uint32_t{1u});

                    outputs.push_back(uint32_t{0u});
                    output_masks.push_back(uint32_t{0u});
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

    inline char const *put_outputs(qmck::logic_array &outputs, qmck::logic_array &output_masks, char const *table_c_str_begin, char const *table_c_str_end)
    {
        char const *it = table_c_str_begin;
        for (; it < table_c_str_end; ++it)
        {
            switch (*it)
            {
            case symbol::HIGH:
            {
                size_t output_end = outputs.size();
                for (size_t output_idx{0}; output_idx < output_end; ++output_idx)
                {
                    outputs[output_idx] <<= 1;
                    outputs[output_idx] |= uint32_t{1u};

                    output_masks[output_idx] <<= 1;
                }

                break;
            }
            case symbol::LOW:
            {
                size_t output_end = outputs.size();
                for (size_t output_idx{0}; output_idx < output_end; ++output_idx)
                {
                    outputs[output_idx] <<= 1;
                    output_masks[output_idx] <<= 1;
                }

                break;
            }
            case symbol::DONT_CARE:
            {
                size_t output_end = outputs.size();
                for (size_t output_idx{0}; output_idx < output_end; ++output_idx)
                {
                    outputs[output_idx] <<= 1;
                    outputs[output_idx] |= uint32_t{1u};

                    output_masks[output_idx] <<= 1;
                    output_masks[output_idx] |= uint32_t{1u};
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

qmck::logic_table::logic_table(qmck::logic_table_format format, qmck::logic_array inputs, qmck::logic_array outputs) : format(std::move(format)), inputs(std::move(inputs)), outputs(std::move(outputs))
{
}

qmck::logic_table::logic_table(qmck::logic_table_format format, qmck::logic_array &&inputs, qmck::logic_array &&outputs) : format(std::move(format)), inputs(inputs), outputs(outputs)
{
}

qmck::logic_table::logic_table(qmck::logic_table_format format, qmck::logic_array inputs, qmck::logic_array outputs, qmck::logic_array output_masks) : format(std::move(format)), inputs(std::move(inputs)), outputs(std::move(outputs)), output_dc_masks(std::move(output_masks))
{
}

qmck::logic_table::logic_table(qmck::logic_table_format format, qmck::logic_array &&inputs, qmck::logic_array &&outputs, qmck::logic_array &&output_masks) : format(std::move(format)), inputs(inputs), outputs(outputs), output_dc_masks(output_masks)
{
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
    swap(first.inputs, second.inputs);
    swap(first.outputs, second.outputs);
    swap(first.output_dc_masks, second.output_dc_masks);
}

qmck::logic_table qmck::parse_logic_table(char const *i, char const *n)
{
    qmck::logic_table table{};
    i = put_logic_table_format(table.format, i, n);

    while (i < n)
    {
        qmck::logic_array row_inputs{};
        qmck::logic_array row_outputs{};
        qmck::logic_array row_output_masks{};

        row_inputs.push_back(uint32_t{0u});
        row_outputs.push_back(uint32_t{0u});
        row_output_masks.push_back(uint32_t{0u});

        i = put_inputs(row_inputs, row_outputs, row_output_masks, i, n);
        i = put_outputs(row_outputs, row_output_masks, i, n);

        table.inputs.insert(table.inputs.end(), row_inputs.begin(), row_inputs.end());
        table.outputs.insert(table.outputs.end(), row_outputs.begin(), row_outputs.end());
        table.output_dc_masks.insert(table.output_dc_masks.end(), row_output_masks.begin(), row_output_masks.end());
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
        for (int i = i_border; i < inputs.size(); ++i)
        {
            if (inputs[i] < inputs[i_min])
            {
                i_min = i;
            }
        }

        uint32_t tmp = inputs[i_border];    // bekommt man den typ auch irgendwie aus dem logic_array heraus?
        inputs[i_border] = inputs[i_min];
        inputs[i_min] = tmp;

        tmp = outputs[i_border];
        outputs[i_border] = outputs[i_min];
        outputs[i_min] = tmp;

        tmp = output_dc_masks[i_border];
        output_dc_masks[i_border] = output_dc_masks[i_min];
        output_dc_masks[i_min] = tmp;
    }
    while (i_border++ < inputs.size());
}
