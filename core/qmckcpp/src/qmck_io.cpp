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
    return out;
}

std::ostream &operator<<(std::ostream &out, const qmck::logic_array &lhs)
{
    return out;
}

std::ostream &operator<<(std::ostream &out, const qmck::logic_table &lhs)
{
    out << "qmck::logic_table{" << std::endl;
    out << '\t' << "logic_table_format=" << lhs.format << ',' << std::endl << std::endl;

    for (int i = 0; i < lhs.inputs.size(); i++)
    {
        out << '\t';
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
