cmake_minimum_required(VERSION 3.13)

set(THIS qmckcpp)

set(INCLUDE
        include/qmck/basic_row.hpp
        include/qmck/generic_table_format.hpp
        include/qmck/logic_table.hpp
        include/qmck/logic_row.hpp
        include/qmck/logic_value.hpp
        include/qmck/quine_row.hpp
        include/qmck/quine_table.hpp
        include/qmck/result_row.hpp
        include/qmck/result_table.hpp
        include/qmck.hpp
        include/qmck_io.hpp
        include/qmck_optional_output.hpp)

set(SRC
        src/qmck/logic_table.cpp
        src/qmck.cpp
        src/qmck_io.cpp)

add_library(${THIS} STATIC ${INCLUDE} ${SRC})
target_compile_options(${THIS} PUBLIC "$<$<CONFIG:DEBUG>:${QMCK_CXX_FLAGS_DEBUG}>")
target_compile_options(${THIS} PUBLIC "$<$<CONFIG:RELEASE>:${QMCK_CXX_FLAGS_RELEASE}>")
target_include_directories(${THIS} PUBLIC include)

option(QMCK_ENABLE_OUTPUT_PROGRESS OFF)
if (QMCK_ENABLE_OUTPUT_PROGRESS)
    target_compile_definitions(${THIS} PRIVATE -DQMCK_ENABLE_OUTPUT_PROGRESS=true)
endif (QMCK_ENABLE_OUTPUT_PROGRESS)
