add_library(easyloggingpp SHARED
        src/easylogging++.cc
        src/easylogging++.h
        )

target_include_directories(easyloggingpp PUBLIC src)
