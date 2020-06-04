cmake_minimum_required(VERSION 3.13)

set(THIS QMCKCPP)
project(${THIS} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/compiler_options.cmake)

set(QMCK_ENABLE_OUTPUT_PROGRESS ON)
add_subdirectory(core/qmckcpp)
add_subdirectory(tools/qmckcpp-cmd)
add_subdirectory(tools/qmckcpp-samplegen)
