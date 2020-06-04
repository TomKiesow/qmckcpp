cmake_minimum_required(VERSION 3.13)

set(THIS qmckcpp-cmd)

set(SRC src/main.cpp)

add_executable(${THIS} ${SRC})
target_compile_options(${THIS} PUBLIC "$<$<CONFIG:DEBUG>:${QMCK_CXX_FLAGS_DEBUG}>")
target_compile_options(${THIS} PUBLIC "$<$<CONFIG:RELEASE>:${QMCK_CXX_FLAGS_RELEASE}>")
target_link_libraries(${THIS} PRIVATE qmckcpp)
