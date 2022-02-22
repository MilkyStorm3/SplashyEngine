#pragma once
#include "Core/Core.hpp"

namespace ant
{

    class GraphicsContext
    {
    public:
        static Scope<GraphicsContext> Create(void *nativeWindow);

        GraphicsContext() {}
        virtual ~GraphicsContext() {}

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };

} // namespace ant