project_add_executable(qmckcpp-cmd)

message(STATUS "${qmckcpp_INC_DIR}")

target_link_libraries(qmckcpp-cmd PRIVATE qmckcpp)
