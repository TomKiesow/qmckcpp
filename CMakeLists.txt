cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

project(QMCKCPP CXX)

add_subdirectory(core/qmckcpp)
add_subdirectory(tools/easyloggingpp)
add_subdirectory(tools/qmckcpp-cmd)
