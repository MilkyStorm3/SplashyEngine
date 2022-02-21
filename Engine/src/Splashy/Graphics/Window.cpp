#include "Pch.h"
#include "Graphics/Window.hpp"
#include <Gl.h>
#include "Utilities/Instrumentation.hpp"
#include "Render/RendererCommands.hpp"
#include "Utilities/InstrumentationMacros.hpp"
#include "Input/Event.hpp"
#include "Core/Core.hpp"

namespace ant
{

    Window::~Window()
    {
        CORE_GENERAL_PROFILE_FUNC();
        glfwDestroyWindow(m_nativeWindow);
    }

    void Window::Update()
    {
        CORE_GENERAL_PROFILE_FUNC();
        glfwSwapBuffers(m_nativeWindow);
        glfwPollEvents();
    }

    void Window::SetResizeability(bool resizeable)
    {
        m_properties.resizeable = resizeable;

        int gl_bool = resizeable ? GL_TRUE : GL_FALSE;

        glfwWindowHint(GLFW_RESIZABLE, gl_bool);
    }

    void Window::SetVsync(bool vsync)
    {
        m_properties.vsync = vsync;

        glfwSwapInterval(int(vsync));
    }

    void Window::SetWindowSize(int width, int height)
    {
        m_properties.width = width;
        m_properties.height = height;
    }

    void Window::Init(const Properties &props)
    {
        CORE_GENERAL_PROFILE_FUNC();
        RendererCommands::Init();        

        m_nativeWindow = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);

        if (m_nativeWindow == NULL)
        {
            CORE_ASSERT(false, "Failed to create GLFW window");
            glfwTerminate();
        }

        glfwMakeContextCurrent(m_nativeWindow);
        RendererCommands::InitApiIfNeeded();

        SetVsync(props.vsync);
        SetResizeability(props.resizeable);

        m_properties = props;

        CORE_ASSERT(m_eventCallback, "Window callback function is not set!");

        // set callbacks
        {
            glfwSetWindowUserPointer(m_nativeWindow, this);

            glfwSetKeyCallback(m_nativeWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods)
                               {
                                   auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
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
                                   }
                               });

            glfwSetMouseButtonCallback(m_nativeWindow, [](GLFWwindow *window, int button, int action, int mods)
                                       {
                                           auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
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
                                           }
                                       });

            glfwSetScrollCallback(m_nativeWindow, [](GLFWwindow *window, double xoffset, double yoffset)
                                  {
                                      auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
                                      auto EventCallback = windowPtr->GetEventCallback();

                                      MouseScrolledEvent e({xoffset, yoffset});
                                      EventCallback(e);
                                  });

            glfwSetCursorPosCallback(m_nativeWindow, [](GLFWwindow *window, double xpos, double ypos)
                                     {
                                         auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
                                         auto EventCallback = windowPtr->GetEventCallback();

                                         MouseMovedEvent e({xpos, ypos});
                                         EventCallback(e);
                                     });

            glfwSetWindowCloseCallback(m_nativeWindow, [](GLFWwindow *window)
                                       {
                                           auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
                                           auto EventCallback = windowPtr->GetEventCallback();

                                           WindowClosedEvent e;
                                           EventCallback(e);
                                       });

            glfwSetWindowFocusCallback(m_nativeWindow, [](GLFWwindow *window, int iconified)
                                       {
                                           auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
                                           auto EventCallback = windowPtr->GetEventCallback();

                                           switch (iconified)
                                           {
                                           case GLFW_TRUE:
                                           {
                                               WindowFocusedEvent e;
                                               EventCallback(e);
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
                                           }
                                       });

            glfwSetWindowSizeCallback(m_nativeWindow, [](GLFWwindow *window, int width, int height)
                                      {
                                          auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
                                          auto EventCallback = windowPtr->GetEventCallback();

                                          WindowRezisedEvent e({width, height});
                                          windowPtr->SetWindowSize(width, height);
                                          glViewport(0, 0, width, height);
                                          EventCallback(e);
                                      });
        }
    }

} // namespace ant