#pragma once
#include <csignal>
#include <stdint.h>
#include "Core/Logger.hpp"
#include "Core/Types.hpp"

namespace df
{

// #define CORE_BREAK() std::raise(SIGINT)

#ifdef __linux__ 

#define CORE_BREAK() std::abort();

#endif

#ifdef _WIN32

#define CORE_BREAK() __debugbreak();

#endif


#define CORE_ASSERT(x, ...)                                \
    if (!(x))                                              \
    {                                                      \
        CORE_ERROR("Assertion failed! {0} ", __VA_ARGS__); \
        CORE_BREAK();                                      \
    }

    template <class _Type>
    using Ref = std::shared_ptr<_Type>;

    template <class _Type, class ..._Args>
    inline Ref<_Type> MakeRef(_Args... args){
        return std::make_shared<_Type>(args...);
    }

    template <class _Type>
    using UniqueRef = std::unique_ptr<_Type>;

    template <class _Type, class ..._Args>
    inline Ref<_Type> MakeUniqueRef(_Args... args){
        return std::make_unique<_Type>(args...);
    }
#define CORE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

} // namespace df
