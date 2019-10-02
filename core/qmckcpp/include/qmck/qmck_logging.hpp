#pragma once

#ifndef QMCK_LOGGING_ENABLED
    #define QMCK_LOGGING_ENABLED false
#endif

#if QMCK_LOGGING_ENABLED
    #include <loguru.hpp>

    #define QMCK_LOG_F(verbosity_name, ...) LOG_F(verbosity_name, __VA_ARGS__)
    #define QMCK_VLOG_F(verbosity, ...) VLOG_F(verbosity, __VA_ARGS__)
    #define QMCK_LOG_IF_F(verbosity_name, cond, fmt, ...) LOG_IF_F(verbosity_name, cond, fmt, __VA_ARGS__)
    #define QMCK_VLOG_IF_F(verbosity, cond, fmt, ...) VLOG_IF_F(verbosity, cond, fmt, __VA_ARGS__)
    #define QMCK_RAW_LOG_F(verbosity_name, fmt, ...) RAW_LOG_F(verbosity_name, fmt, __VA_ARGS__)
    #define QMCK_RAW_VLOG_F(verbosity, fmt, ...) RAW_VLOG_F(verbosity, fmt,__VA_ARGS__)
    #define QMCK_LOG_SCOPE_F(verbosity_name, fmt, ...) LOG_SCOPE_F(verbosity_name, fmt, __VA_ARGS__)
    #define QMCK_VLOG_SCOPE_F(verbosity, fmt, ...) VLOG_SCOPE_F(verbosity, fmt, __VA_ARGS__)
#else
    #define QMCK_LOG_F(verbosity_name, ...) ;
    #define QMCK_VLOG_F(verbosity, ...) ;
    #define QMCK_LOG_IF_F(verbosity_name, cond, fmt, ...) ;
    #define QMCK_VLOG_IF_F(verbosity, cond, fmt, ...) ;
    #define QMCK_RAW_LOG_F(verbosity_name, fmt, ...) ;
    #define QMCK_RAW_VLOG_F(verbosity, fmt, ...) ;
    #define QMCK_LOG_SCOPE_F(verbosity_name, fmt, ...) ;
    #define QMCK_VLOG_SCOPE_F(verbosity, fmt, ...) ;
#endif

