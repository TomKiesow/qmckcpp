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
    out << "qmck::extended_logic_table {" << std::endl;
    out << "logic_table_format=" << lhs.format << ',' << std::endl << std::endl;
    out << "----------------" << std::endl;

    for (int bundle_i{0}; bundle_i < lhs.logic_bundle_ranks.size(); bundle_i++)
    {
        auto &current_bundle = lhs.logic_bundle_ranks[bundle_i];
        if (!current_bundle.inputs.empty())
        {
            for (int row_i{0}; row_i < current_bundle.inputs.size(); row_i++)
            {

                for (int j = lhs.format.inputs_count - 1; j >= 0; j--)
                {
                    if ((current_bundle.input_deduced_masks[row_i] >> j) & 1)
                    {
                        out << 'X';
                    }
                    else
                    {
                        out << ((current_bundle.inputs[row_i] >> j) & 1) ? '1' : '0';
                    }
                }
                out << " | ";

                for (int k = lhs.format.outputs_count - 1; k >= 0; k--)
                {
                    out << ((current_bundle.outputs[row_i] >> k) & 1) ? '1' : '0';
                }
                out << "  ";

                for (int l = lhs.format.outputs_count - 1; l >= 0; l--)
                {
                    out << ((current_bundle.output_done_masks[row_i] >> l) & 1) ? '1' : '0';
                }

                if ((current_bundle.output_done_masks[row_i] & current_bundle.outputs[row_i]) == current_bundle.outputs[row_i])
                {
                    out << " covered";
                }
                else
                {
                    out << " not covered";
                }

                out << std::endl;
            }
            out << "----------------" << std::endl;
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
    out << "qmck::logic_table{" << std::endl;
    out << "logic_table_format=" << lhs.format << ',' << std::endl << std::endl;

    for (int i = 0; i < lhs.inputs.size(); i++)
    {
        for (int j = lhs.format.inputs_count - 1; j >= 0; j--)
        {
            out << ((lhs.inputs[i] >> j) & 1) ? '1' : '0';
        }
        out << " | ";

        for (int k = lhs.format.outputs_count - 1; k >= 0; k--)
        {
            out << ((lhs.outputs[i] >> k) & 1) ? '1' : '0';
        }
        out << "  ";

        for (int l = lhs.format.outputs_count - 1; l >= 0; l--)
        {
            out << ((lhs.output_dc_masks[i] >> l) & 1) ? '1' : '0';
        }
        out << std::endl;
    }

    out << "}";
    return out;
}

std::ostream &operator<<(std::ostream &out, const qmck::logic_table_format &lhs)
{
    out << "qmck::table_format{inputs_count=" << lhs.inputs_count << ", outputs_count=" << lhs.outputs_count << "}";
    return out;
}
