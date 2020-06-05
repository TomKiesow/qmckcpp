cmake_minimum_required(VERSION 3.13)

option(QMCK_ENABLE_SANITIZER_ADDRESS OFF)

if (QMCK_ENABLE_SANITIZER_ADDRESS)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

        set(QMCK_CXX_FLAGS_DEBUG ${QMCK_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer)

    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

        set(QMCK_CXX_FLAGS_DEBUG ${QMCK_CXX_FLAGS_DEBUG} -fsanitize=address)

    else ()

        # error as the sanitizer was explicitly requested!
        message(WARNING
                "Address-Sanitizer not available for ${CMAKE_CXX_COMPILER_ID}! "
                "Consider using one of the following compilers for Address-Sanitizing: Clang, GNU.")

    endif ()

endif ()
