cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

macro(project_add_executable)

    set("__INC_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/inc")
    set("__SRC_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/src")
    file(GLOB "__INC" "${__INC_DIR}/*.h" "${__INC_DIR}/*.hpp")
    file(GLOB "__SRC" "${__SRC_DIR}/*.c" "${__SRC_DIR}/*.cpp")

    # internal only
    #    set("${ARGV0}_INC_DIR"
    #            "${__INC_DIR}"
    #            CACHE PATH "" FORCE)

    # internal only
    #    set("${ARGV0}_SRC_DIR"
    #            "${__SRC_DIR}"
    #            CACHE PATH "" FORCE)

    # internal only
    #    set("${ARGV0}_INC"
    #            "${__INC}"
    #            CACHE String "" FORCE)

    # internal only
    #    set("${ARGV0}_SRC"
    #            "${__SRC}"
    #            CACHE String "" FORCE)

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

    file(GLOB "__INC" "${__INC_DIR}/*.h" "${__INC_DIR}/*.hpp")
    file(GLOB "__INCLUDE" "${__INCLUDE_DIR}/*.h" "${__INCLUDE_DIR}/*.hpp")
    file(GLOB "__SRC" "${__SRC_DIR}/*.c" "${__SRC_DIR}/*.cpp")

    # internal only
    #    set("${ARGV0}_INC_DIR"
    #            "${__INC_DIR}"
    #            CACHE PATH "" FORCE)

    set("${ARGV0}_INCLUDE_DIR"
            "${__INCLUDE_DIR}"
            CACHE PATH "" FORCE)

    # internal only
    #    set("${ARGV0}_SRC_DIR"
    #            "${__SRC_DIR}"
    #            CACHE PATH "" FORCE)

    # internal only
    #    set("${ARGV0}_INC"
    #            "${__INC}"
    #            CACHE String "" FORCE)

    set("${ARGV0}_INCLUDE"
            "${__INCLUDE}"
            CACHE String "" FORCE)

    # internal only
    #    set("${ARGV0}_SRC"
    #            "${__SRC}"
    #            CACHE String "" FORCE)

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
