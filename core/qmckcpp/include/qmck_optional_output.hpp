#pragma once

#ifndef QMCK_ENABLE_OUTPUT_PROGRESS
    #define QMCK_ENABLE_OUTPUT_PROGRESS false
#endif

#if QMCK_ENABLE_OUTPUT_PROGRESS
    #define QMCK_OUTPUT_PROGRESS(expression) expression
#else
    #define QMCK_OUTPUT_PROGRESS(expression)
#endif
