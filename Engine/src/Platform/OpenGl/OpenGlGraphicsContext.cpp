#include "OpenGlGraphicsContext.hpp"
#include <Utilities/InstrumentationMacros.hpp>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

namespace ant::OpenGl
{
    namespace Error
    {
        void ErrorFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
    }

    GlGraphicsContext::GlGraphicsContext(void *nativeWindow)
        : m_nativeWindow(nativeWindow)
    {
        CORE_ASSERT(m_nativeWindow, "Window pointer cannot be null!");
    }

    GlGraphicsContext::~GlGraphicsContext()
    {
    }

    void GlGraphicsContext::Init()
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();

        // todo initialize opengl context using glew

        glfwMakeContextCurrent((GLFWwindow *)m_nativeWindow);
        CORE_ASSERT(glewInit() == GLEW_OK, "Failed to initialize glew!");

        CORE_INFO("OpenGl specs:");
        CORE_INFO(" Vendor {0}", glGetString(GL_VENDOR));
        CORE_INFO(" Renderer {0}", glGetString(GL_RENDERER));
        CORE_INFO(" Version {0}", glGetString(GL_VERSION));
        CORE_INFO("Glfw version: {0}",glfwGetVersionString());

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(&ant::OpenGl::Error::ErrorFunc, NULL);
    }

    void GlGraphicsContext::SwapBuffers()
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        glfwSwapBuffers((GLFWwindow *)m_nativeWindow);
    }
}