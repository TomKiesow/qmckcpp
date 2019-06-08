#include <qmck_io.hpp>

std::ostream &operator<<(std::ostream &out, const qmck::quine_row &lhs)
{


    return out;
}

std::ostream &operator<<(std::ostream &out, const qmck::quine_table &lhs)
{
    out << "qmck::quine_table {\n";
    out << "generic_table_format=" << lhs.format << ",\n\n";
    out << "----------------\n";

    for (size_t bundle_i{0}; bundle_i < lhs.ranks.size(); ++bundle_i)
    {
        auto const &current_bundle = lhs.ranks[bundle_i];
        if (!current_bundle.empty())
        {
            for (size_t row_i{0}; row_i < current_bundle.size(); ++row_i)
            {
                for (int j{0}; j < lhs.format.inputs_count; ++j)
                {
                    if ((current_bundle[row_i].inputs_deduced_mask >> (lhs.format.inputs_count - j - 1)) & 1u)
                    {
                        out << 'X';
                    }
                    else
                    {
                        out << ((current_bundle[row_i].inputs >> (lhs.format.inputs_count - j - 1)) & 1u) ? '1' : '0';
                    }
                }
                out << " | ";

                for (int k{0}; k < lhs.format.outputs_count; ++k)
                {
                    out << ((current_bundle[row_i].outputs >> (lhs.format.outputs_count - k - 1)) & 1u) ? '1' : '0';
                }
                out << "  ";

                for (int l{0}; l < lhs.format.outputs_count; ++l)
                {
                    out << ((current_bundle[row_i].outputs_done_mask >> (lhs.format.outputs_count - l - 1)) & 1u) ? '1' : '0';
                }

                if ((current_bundle[row_i].outputs_done_mask & current_bundle[row_i].outputs) == current_bundle[row_i].outputs)
                {
                    out << " covered";
                }
                else
                {
                    out << " not covered";
                }

                out << '\n';
            }
            out << "----------------\n";
        }
    }

    out << '}';
    return out;
}

std::ostream &operator<<(std::ostream &out, const qmck::logic_row &lhs)
{
    return out;
}

std::ostream &operator<<(std::ostream &out, const qmck::logic_table &lhs)
{
    out << "qmck::logic_table{\n";
    out << "generic_table_format=" << lhs.format << ",\n\n";

    for (size_t i = 0; i < lhs.rows.size(); ++i)
    {
        for (int j{0}; j < lhs.format.inputs_count; ++j)
        {
            out << ((lhs.rows[i].inputs >> (lhs.format.inputs_count - j - 1)) & 1u) ? '1' : '0';
        }
        out << " | ";

        for (int k{0}; k < lhs.format.outputs_count; ++k)
        {
            out << ((lhs.rows[i].outputs >> (lhs.format.outputs_count - k - 1)) & 1u) ? '1' : '0';
        }
        out << "  ";

        for (int l{0}; l < lhs.format.outputs_count; ++l)
        {
            out << ((lhs.rows[i].outputs_dc_mask >> (lhs.format.outputs_count - l - 1)) & 1u) ? '1' : '0';
        }
        out << '\n';
    }

    out << "}";
    return out;
}

std::ostream &operator<<(std::ostream &out, const qmck::generic_table_format &lhs)
{
    out << "qmck::table_format{inputs_count=" << lhs.inputs_count << ", outputs_count=" << lhs.outputs_count << "}";
    return out;
}
