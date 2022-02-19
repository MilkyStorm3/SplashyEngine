#pragma once
#include <Render/RendererCommands.hpp>

namespace ant::OpenGl
{

    class GlRendererCommands : public ant::RendererCommands
    {
    public:
        GlRendererCommands() {}
        ~GlRendererCommands() {}

    private:
        virtual void Init_IMPL() override;
        virtual void InitApiIfNeeded_IMPL() override;
        virtual void Shutdown_IMPL() override;
        virtual void SetClearColor_IMPL(const glm::vec4 &color) override;
        virtual void Clear_IMPL() override;
        virtual void Clear_IMPL(const glm::vec4 &color) override;
    };

} // namespace ant::OpenGl
