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

    enum class DrawMode
    {
        None = 0,
        Triangles,
        Lines
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
        static void SetBlendingMode(BlendingMode source, BlendingMode current);
        static void SetLineThickness(float thickness);

        template <DrawMode mode>
        static inline void DrawIndexed(Ref<Shader> shader, Ref<VertexBuffer> vertices, Ref<IndexBuffer> indices)
        {
            CORE_ASSERT(s_instance, "Renderer commands not initialized");
            s_instance->DrawIndexed_IMPL(shader, vertices, indices, mode);
        }

        template <DrawMode mode>
        static inline void DrawIndexed(Ref<Shader> shader, Ref<VertexArray> verticies)
        {
            CORE_ASSERT(s_instance, "Renderer commands not initialized");
            s_instance->DrawIndexed_IMPL(shader, verticies, mode);
        }

        template <DrawMode mode>
        static inline void DrawUnIndexed(Ref<Shader> shader, Ref<VertexBuffer> vertices)
        {
            CORE_ASSERT(s_instance, "Renderer commands not initialized");
            s_instance->DrawUnIndexed_IMPL(shader, vertices, mode);
        }

    protected:
        virtual void Init_IMPL() = 0;
        virtual void Shutdown_IMPL() = 0;
        virtual void SetClearColor_IMPL(const glm::vec4 &color) = 0;
        virtual void Clear_IMPL() = 0;
        virtual void Clear_IMPL(const glm::vec4 &color) = 0;
        virtual void DrawIndexed_IMPL(Ref<Shader> shader, Ref<VertexArray> verticies, DrawMode mode) = 0;
        virtual void DrawIndexed_IMPL(const Ref<Shader> &shader, const Ref<VertexBuffer> &vertices, const Ref<IndexBuffer> &indices, DrawMode mode) = 0;
        virtual void DrawUnIndexed_IMPL(Ref<Shader> shader, Ref<VertexBuffer> vertices, DrawMode mode) = 0;
        virtual void SetBlendingMode_IMPL(BlendingMode source, BlendingMode current) = 0;
        virtual void SetLineThickness_IMPL(float thickness) = 0;

    private:
        static std::unique_ptr<RendererCommands> s_instance;
    };

}