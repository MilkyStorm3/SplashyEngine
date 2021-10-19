#include "RendererCommands.hpp"
#include "debug/GlErrorHandler.hpp"
#include <Gl.h>
#include "Core/Core.hpp"
#include <stb_image.h>

namespace ant
{
    bool RendererCommands::InitGlfw()
    {
        static bool initialized = false;
        if (!initialized)
        {
            CORE_ASSERT(glfwInit(), "Failed to initialize GLFW ");
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            stbi_set_flip_vertically_on_load(true);
            initialized = true;
        }
        return initialized;
    }

    bool RendererCommands::ShutdownGlfw()
    {
        glfwTerminate();
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
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glClearDepth(1.f);
            glDepthFunc(GL_LEQUAL);

            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(&ant::GlErrorHandler::GlErrorCallbackFunction, NULL);
            initialized = true;
        }
        return initialized;
    }
}