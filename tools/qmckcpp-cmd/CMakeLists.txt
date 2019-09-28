add_executable(qmckcpp-cmd
        src/main.cpp
        )

target_link_libraries(qmckcpp-cmd PRIVATE qmckcpp)
target_link_libraries(qmckcpp-cmd PRIVATE loguru)
