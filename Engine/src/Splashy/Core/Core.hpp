#pragma once
#include <csignal>
#include <stdint.h>
#include "Core/Logger.hpp"
#include "Core/Types.hpp"
#include "Core/Memory.hpp"
#include "Core/Macros.hpp"

#ifdef SPL_ENABLE_ASSERTS

#define CORE_ASSERT(x, ...)      \
    if (!(x))                    \
    {                            \
        CORE_ERROR(__VA_ARGS__); \
        CORE_BREAK();            \
    }

#else

#define CORE_ASSERT(x, ...) ;

#endif
