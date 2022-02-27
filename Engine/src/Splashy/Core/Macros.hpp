#pragma once
#include <csignal>

#ifdef __linux__

#define CORE_BREAK() std::raise(SIGTRAP)

#endif

#ifdef _WIN32

#define CORE_BREAK() __debugbreak();

#endif

#define CORE_BIND_EVENT_FN(target, fn) std::bind(&fn, target, std::placeholders::_1)
