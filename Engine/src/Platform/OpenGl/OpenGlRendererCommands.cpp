#include "OpenGlRendererCommands.hpp"
#include <Gl.h>
#include "Core/Core.hpp"
#include <stb_image.h>
#include <Utilities/InstrumentationMacros.hpp>

#include "Core/Logger.hpp"
namespace ant::OpenGl
{

    void GlRendererCommands::Init_IMPL()
    {
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

}