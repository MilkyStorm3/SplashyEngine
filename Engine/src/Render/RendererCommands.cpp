#include "RendererCommands.hpp"
#include "debug/GlErrorHandler.hpp"
#include <Gl.h>
#include "Core/Core.hpp"
#include <stb_image.h>
#include "debug/Instrumentation.hpp"

namespace ant
{
    bool RendererCommands::InitGlfw()
    {
        static bool initialized = false;
        if (!initialized)
        {
            CORE_ASSERT(glfwInit(), "Failed to initialize GLFW ");
            // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            stbi_set_flip_vertically_on_load(true);
            initialized = true;
        }
        return initialized;
    }

    bool RendererCommands::ShutdownGlfw()
    {
        glfwTerminate();
    }

    void RendererCommands::SetClearColor(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RendererCommands::Clear()
    {
        CORE_PROFILE_FUNC();
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    void RendererCommands::Clear(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        Clear();
    }

    bool RendererCommands::InitGlew()
    {
        static bool initialized = false;
        if (!initialized)
        {
            GLenum glewInitState = glewInit();
            CORE_ASSERT(glewInitState == GLEW_OK, "Failed to initialize glew");
            initialized = true;
        }
        return initialized;
    }

    bool RendererCommands::EnableGlDebugMessages()
    {
        static bool initialized = false;
        if (!initialized)
        {
            CORE_INFO("Started OpenGL {0}", glGetString(GL_VERSION));
            // glEnable(GL_BLEND);
            // glEnable(GL_DEPTH_TEST);
            // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // glClearDepth(1.f);
            // glDepthFunc(GL_LEQUAL);

            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(&ant::GlErrorHandler::ErrorFunc, NULL);
            initialized = true;
        }
        return initialized;
    }
}