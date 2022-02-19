#include "OpenGlRendererCommands.hpp"
#include <Gl.h>
#include "Core/Core.hpp"
#include <stb_image.h>
#include "debug/Instrumentation.hpp"

namespace ant::OpenGl
{
    namespace Error
    {
        void ErrorFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
    }

    void GlRendererCommands::Init_IMPL()
    {
        static bool initialized = false;
        if (!initialized)
        {
            CORE_ASSERT(glfwInit(), "Failed to initialize GLFW ");
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            CORE_INFO("Started OpenGL {0}", glGetString(GL_VERSION));

            // glEnable(GL_BLEND);
            // glEnable(GL_DEPTH_TEST);
            // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // glClearDepth(1.f);
            // glDepthFunc(GL_LEQUAL);

            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(&Error::ErrorFunc, NULL);

            stbi_set_flip_vertically_on_load(true);
            initialized = true;
        }
    }

    void GlRendererCommands::Shutdown_IMPL()
    {
        glfwTerminate();
    }

    void GlRendererCommands::SetClearColor_IMPL(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void GlRendererCommands::Clear_IMPL()
    {
        CORE_PROFILE_FUNC();
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    void GlRendererCommands::Clear_IMPL(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        Clear();
    }

    void GlRendererCommands::InitGlewIfNeeded_IMPL()
    {
        static bool initialized = false;
        if (!initialized)
        {
            GLenum glewInitState = glewInit();
            CORE_ASSERT(glewInitState == GLEW_OK, "Failed to initialize glew");
            initialized = true;
        }
    }

}