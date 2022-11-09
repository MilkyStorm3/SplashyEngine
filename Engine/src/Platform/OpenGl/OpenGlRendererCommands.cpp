#include "OpenGlRendererCommands.hpp"
#include <GL/glew.h>
#include "Core/Core.hpp"
#include <stb_image.h>
#include <Utilities/InstrumentationMacros.hpp>

#include "Core/Logger.hpp"
namespace ant::OpenGl
{
    namespace Utils
    {

        static GLenum BlendingModeToGL(BlendingMode mode)
        {

            switch (mode)
            {
            case BlendingMode::Zero:
                return GL_ZERO;
            case BlendingMode::One:
                return GL_ONE;

            case BlendingMode::Source:
                return GL_SRC_COLOR;
            case BlendingMode::Current:
                return GL_DST_COLOR;

            case BlendingMode::OneMinusSource:
                return GL_ONE_MINUS_SRC_COLOR;
            case BlendingMode::OneMinusCurrent:
                return GL_ONE_MINUS_DST_COLOR;

            case BlendingMode::SourceAlpha:
                return GL_SRC_ALPHA;
            case BlendingMode::CurrentAlpha:
                return GL_DST_ALPHA;

            case BlendingMode::OneMinusSourceAlpha:
                return GL_ONE_MINUS_SRC_ALPHA;
            case BlendingMode::OneMinusCurrentAlpha:
                return GL_ONE_MINUS_DST_ALPHA;
            }

            CORE_ASSERT(false, "Unsupported blending mode");
            return 0;
        }

        static GLenum DrawModeToGl(DrawMode mode)
        {
            switch (mode)
            {
            case DrawMode::Triangles:
                return GL_TRIANGLES;
            case DrawMode::Lines:
                return GL_LINES;

            default:
                break;
            }
            CORE_ASSERT(false, "Unsupported draw mode");
            return 0;
        }
    }
    void GlRendererCommands::Init_IMPL()
    {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LINE_SMOOTH);
        glDepthFunc(GL_LEQUAL);
    }

    void GlRendererCommands::Shutdown_IMPL()
    {
    }

    void GlRendererCommands::SetClearColor_IMPL(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void GlRendererCommands::Clear_IMPL()
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    void GlRendererCommands::Clear_IMPL(const glm::vec4 &color)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        glClearColor(color.r, color.g, color.b, color.a);
        Clear();
    }

    void GlRendererCommands::DrawIndexed_IMPL(Ref<Shader> shader, Ref<VertexArray> verticies, DrawMode mode)
    {
        shader->Bind();
        verticies->Bind();

        uint32_t count = verticies->GetIndexBuffer()->GetCount();

        glDrawElements(Utils::DrawModeToGl(mode), count, GL_UNSIGNED_INT, (void *)0);
    }

    void GlRendererCommands::DrawIndexed_IMPL(const Ref<Shader> &shader, const Ref<VertexBuffer> &vertices, const Ref<IndexBuffer> &indices, DrawMode mode)
    {
        shader->Bind();
        vertices->Bind();
        indices->Bind();

        glDrawElements(Utils::DrawModeToGl(mode), indices->GetCount(), GL_UNSIGNED_INT, (void *)0);

        indices->UnBind();
        vertices->UnBind();
        shader->UnBind();
    }

    void GlRendererCommands::DrawUnIndexed_IMPL(Ref<Shader> shader, Ref<VertexBuffer> vertices, DrawMode mode)
    {
        CORE_ASSERT(false, "No implementation");
    }

    void GlRendererCommands::SetBlendingMode_IMPL(BlendingMode source, BlendingMode current)
    {
        glBlendFunc(Utils::BlendingModeToGL(source), Utils::BlendingModeToGL(current));
    }

    void GlRendererCommands::SetLineThickness_IMPL(float thickness)
    {
        glLineWidth(thickness);
    }

}