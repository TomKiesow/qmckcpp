cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS_DEBUG "-O0 -g -Wall -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -DDEBUG")

set(CMAKE_C_FLAGS_RELEASE "-O2 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")

project(QMCKCPP CXX)

set(QMCK_ENABLE_OUTPUT_PROGRESS ON)
add_subdirectory(core/qmckcpp)
add_subdirectory(tools/qmckcpp-cmd)
add_subdirectory(tools/qmckcpp-samplegen)
