cmake_minimum_required(VERSION 3.13)

set(THIS QMCKCPP)
project(${THIS} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

unset(CMAKE_CXX_FLAGS_DEBUG)
unset(CMAKE_CXX_FLAGS_RELEASE)
include(cmake/compiler_sanitizer.cmake)
include(cmake/compiler_warnings.cmake)

add_subdirectory(core/qmckcpp)
add_subdirectory(tools/qmckcpp-cmd)
add_subdirectory(tools/qmckcpp-samplegen)
