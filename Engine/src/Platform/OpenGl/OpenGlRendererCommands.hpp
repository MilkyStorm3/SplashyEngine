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
        virtual void Shutdown_IMPL() override;
        virtual void SetClearColor_IMPL(const glm::vec4 &color) override;
        virtual void Clear_IMPL() override;
        virtual void Clear_IMPL(const glm::vec4 &color) override;
        virtual void DrawIndexed_IMPL(Ref<Shader> shader, Ref<VertexArray> verticies) override;
        virtual void DrawIndexed_IMPL(const Ref<Shader> &shader, const Ref<VertexBuffer> &vertices, const Ref<IndexBuffer> &indices) override;
        virtual void SetBlendingMode_IMPL(BlendingMode source, BlendingMode current) override;
    };

} // namespace ant::OpenGl
