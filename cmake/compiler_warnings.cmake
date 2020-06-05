cmake_minimum_required(VERSION 3.13)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

    set(QMCK_CXX_WARNINGS
            -Wall -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wdouble-promotion
            -Wformat=2)

    set(QMCK_CXX_FLAGS_DEBUG ${QMCK_CXX_FLAGS_DEBUG} ${QMCK_CXX_WARNINGS} -O0 -DDEBUG)
    set(QMCK_CXX_FLAGS_RELEASE ${QMCK_CXX_FLAGS_RELEASE} ${QMCK_CXX_WARNINGS} -O3)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    set(QMCK_CXX_WARNINGS
            -Wall -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wdouble-promotion
            -Wformat=2)

    set(QMCK_CXX_FLAGS_DEBUG ${QMCK_CXX_FLAGS_DEBUG} ${QMCK_CXX_WARNINGS} -O0 -DDEBUG)
    set(QMCK_CXX_FLAGS_RELEASE ${QMCK_CXX_FLAGS_RELEASE} ${QMCK_CXX_WARNINGS} -O3)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    set(QMCK_CXX_WARNINGS
            /W4
            /w14242 /w14254 /w14263
            /w14265 /w14287 /we4289
            /w14296 /w14311 /w14545
            /w14546 /w14547 /w14549
            /w14555 /w14619 /w14640
            /w14826 /w14905 /w14906
            /w14928)

    set(QMCK_CXX_FLAGS_DEBUG ${QMCK_CXX_FLAGS_DEBUG} ${QMCK_CXX_WARNINGS} /MDd /Zi /Ob0 /Od /RTC1)
    set(QMCK_CXX_FLAGS_RELEASE ${QMCK_CXX_FLAGS_RELEASE} ${QMCK_CXX_WARNINGS} /MD /O2 /Ob2)

else ()

    message(${QMCK_WARNING}
            "No Warnings specified for ${CMAKE_CXX_COMPILER_ID}. "
            "Consider using one of the following compilers: Clang, GNU, MSVC.")

endif ()
