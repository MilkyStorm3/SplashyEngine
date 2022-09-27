#include "CommonWindow.hpp"
#include <GLFW/glfw3.h> 
#include <Input/Event.hpp>
#include "CommonInput.hpp"

namespace ant::Common
{
    static size_t s_windowCount = 0;
    void GlfwErrorCallback(int error_code, const char *description);

    CommonWindow::CommonWindow()
    {
    }

    CommonWindow::~CommonWindow()
    {
        glfwDestroyWindow(m_window);
        --s_windowCount;

        if (s_windowCount == 0)
            glfwTerminate();
    }

    void CommonWindow::Init(const std::string &title, uint32_t width, uint32_t height, bool resizeable, bool vsync)
    {

        if (s_windowCount == 0)
        {
            CORE_ASSERT(glfwInit(), "Failed to initialize GLFW ");
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwSetErrorCallback(&GlfwErrorCallback);
        }

        m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

        if (s_windowCount == 0)
            CommonInput::s_activeNativeWindow = m_window;

        if (m_window == NULL)
        {
            CORE_ASSERT(false, "Failed to create GLFW window");
            glfwTerminate();
        }
        ++s_windowCount;

        m_context = GraphicsContext::Create(m_window);
        m_context->Init();
        SetVsync(vsync);

        int res = resizeable ? GL_TRUE : GL_FALSE;
        glfwWindowHint(GLFW_RESIZABLE, res);

        CORE_ASSERT(m_callback, "Window callback function not set. It has to be initialized before Init() call!");

        SetCallbacks();
    }

    void CommonWindow::Update()
    {
        glfwPollEvents();
        m_context->SwapBuffers();
    }

    void CommonWindow::SetEventCallback(const EventCallback &callback)
    {
        m_callback = callback;
    }

    void CommonWindow::SetVsync(bool vsync)
    {
        m_vSync = vsync;
        glfwSwapInterval(int(m_vSync));
    }

    bool CommonWindow::IsVsync() const
    {
        return m_vSync;
    }

    void *CommonWindow::GetNativeWindow() const
    {
        return m_window;
    }

    const Window::EventCallback &CommonWindow::GetEventCallback() const
    {
        return m_callback;
    }

    glm::ivec2 CommonWindow::GetSize() const
    {
        glm::ivec2 size;
        glfwGetWindowSize(m_window, &size.x, &size.y);
        return size;
    }

    void CommonWindow::SetCallbacks()
    {

        glfwSetWindowUserPointer(m_window, this);

        glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
                           {
                                   auto *windowPtr = (CommonWindow *)glfwGetWindowUserPointer(window);
                                   auto EventCallback = windowPtr->GetEventCallback();

                                   KeyCode myKey = KeyCode(key);
                                   KeyModifier myMod = KeyModifier(mods);

                                   switch (action)
                                   {
                                   case GLFW_PRESS:
                                   {
                                       KeyPressedEvent e(myKey, myMod);
                                       EventCallback(e);
                                       break;
                                   }
                                   case GLFW_RELEASE:
                                   {
                                       KeyReleasedEvent e(myKey, myMod);
                                       EventCallback(e);
                                       break;
                                   }
                                   case GLFW_REPEAT:
                                   {
                                       KeyPressedEvent e(myKey, myMod, true);
                                       EventCallback(e);
                                       break;
                                   }
                                   default:
                                       break;
                                   } });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int mods)
                                   {
                                           auto *windowPtr = (CommonWindow *)glfwGetWindowUserPointer(window);
                                           auto EventCallback = windowPtr->GetEventCallback();

                                           auto myButton = MouseButtonCode(button);
                                           KeyModifier myMod = KeyModifier(mods);
                                           switch (action)
                                           {
                                           case GLFW_PRESS:
                                           {
                                               MouseButtonPressedEvent e(myButton, myMod);
                                               EventCallback(e);
                                               break;
                                           }
                                           case GLFW_RELEASE:
                                           {
                                               MouseButtonReleasedEvent e(myButton, myMod);
                                               EventCallback(e);
                                               break;
                                           }

                                           default:
                                               break;
                                           } });

        glfwSetScrollCallback(m_window, [](GLFWwindow *window, double xoffset, double yoffset)
                              {
                                      auto *windowPtr = (CommonWindow *)glfwGetWindowUserPointer(window);
                                      auto EventCallback = windowPtr->GetEventCallback();

                                      MouseScrolledEvent e({xoffset, yoffset});
                                      EventCallback(e); });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double xpos, double ypos)
                                 {
                                         auto *windowPtr = (CommonWindow *)glfwGetWindowUserPointer(window);
                                         auto EventCallback = windowPtr->GetEventCallback();

                                         MouseMovedEvent e({xpos, ypos});
                                         EventCallback(e); });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window)
                                   {
                                           auto *windowPtr = (CommonWindow *)glfwGetWindowUserPointer(window);
                                           auto EventCallback = windowPtr->GetEventCallback();

                                           WindowClosedEvent e;
                                           EventCallback(e); });

        glfwSetWindowFocusCallback(m_window, [](GLFWwindow *window, int iconified)
                                   {
                                           auto *windowPtr = (CommonWindow *)glfwGetWindowUserPointer(window);
                                           auto EventCallback = windowPtr->GetEventCallback();

                                           switch (iconified)
                                           {
                                           case GLFW_TRUE:
                                           {
                                               WindowFocusedEvent e;
                                               EventCallback(e);
                                               CommonInput::s_activeNativeWindow = window;
                                               break;
                                           }
                                           case GLFW_FALSE:
                                           {
                                               WindowUnfocusedEvent e;
                                               EventCallback(e);
                                               break;
                                           }
                                           default:
                                               break;
                                           } });

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int width, int height)
                                  {
                                          auto *windowPtr = (CommonWindow *)glfwGetWindowUserPointer(window);
                                          auto EventCallback = windowPtr->GetEventCallback();

                                          WindowRezisedEvent e({width, height});
                                          glViewport(0, 0, width, height);
                                          EventCallback(e); });
    }
}