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
                for (std::uint32_t j{0}; j < lhs.format.inputs_count; ++j)
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

                for (std::uint32_t k{0}; k < lhs.format.outputs_count; ++k)
                {
                    out << ((current_bundle[row_i].outputs >> (lhs.format.outputs_count - k - 1)) & 1u) ? '1' : '0';
                }
                out << "  ";

                for (std::uint32_t l{0}; l < lhs.format.outputs_count; ++l)
                {
                    out << ((current_bundle[row_i].outputs_done_mask >> (lhs.format.outputs_count - l - 1)) & 1u) ? '1' : '0';
                }

                if ((current_bundle[row_i].outputs_done_mask & current_bundle[row_i].outputs) == current_bundle[row_i].outputs)
                {
                    out << " (covered) ";
                }
                else
                {
                    out << " (not covered) ";
                }

                auto minterms_covered = current_bundle[row_i].calculate_minterms_covered();
                for (auto &minterm : minterms_covered)
                {
                    out << minterm << ',';
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
        for (std::uint32_t j{0}; j < lhs.format.inputs_count; ++j)
        {
            out << ((lhs.rows[i].inputs >> (lhs.format.inputs_count - j - 1)) & 1u) ? '1' : '0';
        }
        out << " | ";

        for (std::uint32_t k{0}; k < lhs.format.outputs_count; ++k)
        {
            out << ((lhs.rows[i].outputs >> (lhs.format.outputs_count - k - 1)) & 1u) ? '1' : '0';
        }
        out << "  ";

        for (std::uint32_t l{0}; l < lhs.format.outputs_count; ++l)
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

std::ostream &operator<<(std::ostream &out, const qmck::result_table &lhs)
{
    out << "qmck::result_table {\n";
    out << "generic_table_format=" << lhs.format << ",\n\n";

    for (auto pair:lhs.rows)
    {
        auto &quine_row = pair.second;
        auto &id = pair.first;

        out << "id:" << id << "  ";

        for (std::uint32_t j{0}; j < lhs.format.inputs_count; ++j)
        {
            if ((quine_row.inputs_deduced_mask >> (lhs.format.inputs_count - j - 1)) & 1u)
            {
                out << 'X';
            }
            else
            {
                out << ((quine_row.inputs >> (lhs.format.inputs_count - j - 1)) & 1u) ? '1' : '0';
            }
        }
        out << " | ";

        for (std::uint32_t k{0}; k < lhs.format.outputs_count; ++k)
        {
            out << ((quine_row.outputs >> (lhs.format.outputs_count - k - 1)) & 1u) ? '1' : '0';
        }
        out << "  ";

        auto minterms_covered = quine_row.calculate_minterms_covered();
        for (auto &minterm : minterms_covered)
        {
            out << minterm << ',';
        }

        out << "\n";
    }

    out << '}';
    return out;
}

std::ostream &operator<<(std::ostream &out, qmck::tree::rootnode &root)
{
    // loop until -1 and print last child seperately so there are no trailing operation signs
    auto &children = root.get_children();
    for (size_t child_i{0}; child_i < children.size() - 1; ++child_i)
    {
        auto &child = children[child_i];
        if (child->get_children().empty())
        {
            out << child->operand << " * ";
        }
        else
        {
            out << "(";
            recursive_tree_cout(out, child, 1);
            out << ") * ";
        }
    }

    auto &child = children[children.size() - 1];
    if (child->get_children().empty())
    {
        out << child->operand;
    }
    else
    {
        out << "(";
        recursive_tree_cout(out, child, 1);
        out << ")";
    }

    return out;
}

std::ostream &recursive_tree_cout(std::ostream &out, qmck::tree::treenode *lhs, int depth)
{
    std::string operation = depth % 2 == 1 ? "+" : "*";
    // loop until -1 and print last child seperately so there are no trailing operation signs
    auto &children = lhs->get_children();
    for (size_t child_i{0}; child_i < children.size() - 1; ++child_i)
    {
        auto &child = children[child_i];
        if (child->get_children().empty())
        {
            out << child->operand << operation;
        }
        else
        {
            out << "(";
            recursive_tree_cout(out, child, depth + 1);
            out << ")" << operation;
        }
    }

    auto &child = children[children.size() - 1];
    if (child->get_children().empty())
    {
        out << child->operand;
    }
    else
    {
        out << "(";
        recursive_tree_cout(out, child, depth + 1);
        out << ")";
    }

    return out;
}

