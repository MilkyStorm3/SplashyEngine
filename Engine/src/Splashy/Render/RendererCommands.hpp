#pragma once
#include <glm/vec4.hpp>
#include <memory>
#include "Graphics/VertexArray.hpp"
#include "Graphics/Shader.hpp"

namespace ant
{

    enum class RenderApi
    {
        None = 0,
        OpenGl,
        Vulcan
    };

    enum class BlendingMode
    {
        None = 0,
        Zero,
        One,
        Source,
        Current,
        OneMinusSource,
        OneMinusCurrent,
        SourceAlpha,
        OneMinusSourceAlpha,
        CurrentAlpha,
        OneMinusCurrentAlpha
    };

    class RendererCommands
    {
    public:
        RendererCommands() {}
        virtual ~RendererCommands() {}

        // exposed functions
        static void Init();
        static void Shutdown();
        static void SetClearColor(const glm::vec4 &color);
        static void Clear();
        static void Clear(const glm::vec4 &color);
        static RenderApi GetRenderApi();
        static void DrawIndexed(Ref<Shader> shader, Ref<VertexBuffer> vertices, Ref<IndexBuffer> indices);
        static void DrawIndexed(Ref<Shader> shader, Ref<VertexArray> verticies);
        static void SetBlendingMode(BlendingMode source, BlendingMode current);

    protected:
        virtual void Init_IMPL() = 0;
        virtual void Shutdown_IMPL() = 0;
        virtual void SetClearColor_IMPL(const glm::vec4 &color) = 0;
        virtual void Clear_IMPL() = 0;
        virtual void Clear_IMPL(const glm::vec4 &color) = 0;
        virtual void DrawIndexed_IMPL(Ref<Shader> shader, Ref<VertexArray> verticies) = 0;
        virtual void DrawIndexed_IMPL(const Ref<Shader> &shader, const Ref<VertexBuffer> &vertices, const Ref<IndexBuffer> &indices) = 0;
        virtual void SetBlendingMode_IMPL(BlendingMode source, BlendingMode current) = 0;

    private:
        static std::unique_ptr<RendererCommands> s_instance;
    };

}