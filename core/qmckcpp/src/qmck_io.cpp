#include <qmck_io.hpp>

std::ostream &operator<<(std::ostream &out, const qmck::extended_logic_bundle &lhs)
{
    out << "qmck::extended_logic_bundle {\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "    " << lhs.inputs << "\n";
    out << "}\n";

    return out;
}

std::ostream &operator<<(std::ostream &out, const qmck::extended_logic_table &lhs)
{
    out << "qmck::extended_logic_table {\n";
    out << "logic_table_format=" << lhs.format << ",\n\n";
    out << "----------------\n";

    for (size_t bundle_i{0}; bundle_i < lhs.logic_bundle_ranks.size(); ++bundle_i)
    {
        auto const &current_bundle = lhs.logic_bundle_ranks[bundle_i];
        if (!current_bundle.inputs.empty())
        {
            for (size_t row_i{0}; row_i < current_bundle.inputs.size(); ++row_i)
            {
                for (int j{0}; j < lhs.format.inputs_count; ++j)
                {
                    if ((current_bundle.input_deduced_masks[row_i] >> (lhs.format.inputs_count - j)) & 1)
                    {
                        out << 'X';
                    }
                    else
                    {
                        out << ((current_bundle.inputs[row_i] >> (lhs.format.inputs_count - j)) & 1) ? '1' : '0';
                    }
                }
                out << " | ";

                for (int k{0}; k < lhs.format.outputs_count; ++k)
                {
                    out << ((current_bundle.outputs[row_i] >> (lhs.format.outputs_count - k)) & 1) ? '1' : '0';
                }
                out << "  ";

                for (int l{0}; l < lhs.format.outputs_count; ++l)
                {
                    out << ((current_bundle.output_done_masks[row_i] >> (lhs.format.outputs_count - l)) & 1) ? '1' : '0';
                }

                if ((current_bundle.output_done_masks[row_i] & current_bundle.outputs[row_i]) == current_bundle.outputs[row_i])
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

std::ostream &operator<<(std::ostream &out, const qmck::logic_array &lhs)
{
    return out;
}

std::ostream &operator<<(std::ostream &out, const qmck::logic_table &lhs)
{
    out << "qmck::logic_table{\n";
    out << "logic_table_format=" << lhs.format << ",\n\n";

    for (size_t i = 0; i < lhs.inputs.size(); ++i)
    {
        for (int j{0}; j < lhs.format.inputs_count; ++j)
        {
            out << ((lhs.inputs[i] >> (lhs.format.inputs_count - j)) & 1) ? '1' : '0';
        }
        out << " | ";

        for (int k{0}; k < lhs.format.outputs_count; ++k)
        {
            out << ((lhs.outputs[i] >> (lhs.format.outputs_count - k)) & 1) ? '1' : '0';
        }
        out << "  ";

        for (int l{0}; l < lhs.format.outputs_count; ++l)
        {
            out << ((lhs.output_dc_masks[i] >> (lhs.format.outputs_count - l)) & 1) ? '1' : '0';
        }
        out << '\n';
    }

    out << "}";
    return out;
}

std::ostream &operator<<(std::ostream &out, const qmck::logic_table_format &lhs)
{
    out << "qmck::table_format{inputs_count=" << lhs.inputs_count << ", outputs_count=" << lhs.outputs_count << "}";
    return out;
}
