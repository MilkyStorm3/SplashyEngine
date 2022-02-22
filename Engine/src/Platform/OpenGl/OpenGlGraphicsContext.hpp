#pragma once
#include <Graphics/GraphicsContext.hpp>

namespace ant::OpenGl
{

    class GlGraphicsContext : public ant::GraphicsContext
    {
    public:
        GlGraphicsContext(void *nativeWindow);
        ~GlGraphicsContext();
    
        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        void* m_nativeWindow;
    };

} // namespace ant::OpenGl
