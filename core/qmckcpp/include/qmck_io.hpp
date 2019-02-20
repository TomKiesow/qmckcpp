#include <qmck.hpp>
#include <ostream>

std::ostream &operator<<(std::ostream &out, const qmck::extended_logic_bundle &lhs);

std::ostream &operator<<(std::ostream &out, const qmck::extended_logic_table &lhs);

std::ostream &operator<<(std::ostream &out, const qmck::logic_array &lhs);

std::ostream &operator<<(std::ostream &out, const qmck::logic_table &lhs);

std::ostream &operator<<(std::ostream &out, const qmck::logic_table_format &lhs);
