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
    return out;
}

std::ostream &operator<<(std::ostream &out, const qmck::logic_table_format &lhs)
{
    return out;
}
