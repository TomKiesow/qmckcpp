add_library(qmckcpp SHARED
        # headers
        include/qmck.hpp
        include/qmck_io.hpp

        include/qmck/basic_row.hpp
        include/qmck/generic_table_format.hpp
        include/qmck/logic_table.hpp
        include/qmck/logic_row.hpp
        include/qmck/logic_value.hpp
        include/qmck/quine_row.hpp
        include/qmck/quine_table.hpp
        include/qmck/result_row.hpp
        include/qmck/result_table.hpp

        include/qmck/tree/node.hpp
        include/qmck/tree/tree.hpp
        include/qmck/tree/tree_utils.hpp

        # sources
        src/qmck.cpp
        src/qmck_io.cpp

        src/qmck/basic_row.cpp
        src/qmck/generic_table_format.cpp
        src/qmck/logic_table.cpp
        src/qmck/quine_row.cpp
        src/qmck/quine_table.cpp
        src/qmck/result_row.cpp
        src/qmck/result_table.cpp

        src/qmck/tree/node.cpp
        src/qmck/tree/tree.cpp
        src/qmck/tree/tree_utils.cpp
        )

target_include_directories(qmckcpp PUBLIC include)
