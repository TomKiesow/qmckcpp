cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

macro(project_add_executable)

    set("__INC_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/inc")
    set("__SRC_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/src")

    file(GLOB_RECURSE "__INC" "${__INC_DIR}/*.h" "${__INC_DIR}/*.hpp")
    file(GLOB_RECURSE "__SRC" "${__SRC_DIR}/*.c" "${__SRC_DIR}/*.cpp")

    add_executable("${ARGV0}"
            "${__INC}"
            "${__SRC}")

    target_include_directories("${ARGV0}" PRIVATE "${__INC_DIR}")
    set_target_properties("${ARGV0}" PROPERTIES LINKER_LANGUAGE CXX)

endmacro()

macro(project_add_library)

    set("__INC_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/inc")
    set("__INCLUDE_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/include")
    set("__SRC_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/src")

    file(GLOB_RECURSE "__INC" "${__INC_DIR}/*.h" "${__INC_DIR}/*.hpp")
    file(GLOB_RECURSE "__INCLUDE" "${__INCLUDE_DIR}/*.h" "${__INCLUDE_DIR}/*.hpp")
    file(GLOB_RECURSE "__SRC" "${__SRC_DIR}/*.c" "${__SRC_DIR}/*.cpp")

    add_library("${ARGV0}" SHARED
            "${__INC}"
            "${__INCLUDE}"
            "${__SRC}")

    target_include_directories("${ARGV0}" PRIVATE "${__INC_DIR}")
    target_include_directories("${ARGV0}" PUBLIC "${__INCLUDE_DIR}")
    set_target_properties("${ARGV0}" PROPERTIES LINKER_LANGUAGE CXX)

endmacro()

project(QMCKCPP CXX)

add_subdirectory(core/qmckcpp)
add_subdirectory(tools/qmckcpp-cmd)
