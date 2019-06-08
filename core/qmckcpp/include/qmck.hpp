#pragma once

#include <qmck/quine_row.hpp>
#include <qmck/quine_table.hpp>
#include <qmck/logic_value.hpp>
#include <qmck/logic_row.hpp>
#include <qmck/logic_table.hpp>
#include <qmck/generic_table_format.hpp>
#include <qmck_io.hpp>

namespace qmck
{
    qmck::quine_table deduce(qmck::logic_table&);
}
