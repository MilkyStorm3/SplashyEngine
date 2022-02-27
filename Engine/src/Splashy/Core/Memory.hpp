#pragma once
#include <memory>

namespace ant
{

    template <class _Type>
    using Ref = std::shared_ptr<_Type>;

    template <class _Type, class... _Args>
    inline Ref<_Type> MakeRef(_Args... args)
    {
        return std::make_shared<_Type>(args...);
    }

    template <class _Type>
    using Scope = std::unique_ptr<_Type>;

    template <class _Type, class... _Args>
    inline Scope<_Type> MakeScope(_Args... args)
    {
        return std::make_unique<_Type>(args...);
    }

}
