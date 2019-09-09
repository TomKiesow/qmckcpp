#include <qmck.hpp>
#include <qmck/tree/tree.hpp>
#include <ostream>

std::ostream &operator<<(std::ostream &out, const qmck::quine_row &lhs);

std::ostream &operator<<(std::ostream &out, const qmck::quine_table &lhs);

std::ostream &operator<<(std::ostream &out, const qmck::logic_row &lhs);

std::ostream &operator<<(std::ostream &out, const qmck::logic_table &lhs);

std::ostream &operator<<(std::ostream &out, const qmck::generic_table_format &lhs);

std::ostream &operator<<(std::ostream &out, const qmck::result_table &lhs);

std::ostream &operator<<(std::ostream &out, qmck::tree::tree &lhs);

std::ostream &recursive_tree_cout(std::ostream &out, qmck::tree::node *lhs);
