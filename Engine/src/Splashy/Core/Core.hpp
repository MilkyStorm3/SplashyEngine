#pragma once
#include <csignal>
#include <stdint.h>
#include "Core/Logger.hpp"
#include "Core/Types.hpp"
#include "Core/Memory.hpp"
#include "Core/Macros.hpp"

#if !defined(__linux__) && !defined(_WIN32)
    #error Unsupported platform!
#endif

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
    #define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#ifdef SPL_ENABLE_ASSERTS

    #define CORE_ASSERT(x, ...)      \
        if (!(x))                    \
        {                            \
            CORE_ERROR(__VA_ARGS__); \
            CORE_BREAK();            \
        }

    #define NO_IMPLEMENTATION CORE_ASSERT(false, __PRETTY_FUNCTION__ " HAS NO IMPLEMENTATION");

#else

    #define CORE_ASSERT(x, ...) ;
    #define NO_IMPLEMENTATION 

#endif


#if defined(SPL_BUILD_SHARED) && defined(_WIN32)

    #ifdef SPL_CORE
        #define SPL_DEF  __declspec(dllexport)
    #else
        #define SPL_DEF __declspec(dllimport)
    #endif

#else
    #define SPL_DEF 
#endif

