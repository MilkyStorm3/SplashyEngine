#include "Pch.h"
#include "Graphics/Window.hpp"
#include <Gl.h>
#include "debug/Instrumentation.hpp"
#include "Render/RendererCommands.hpp"

namespace ant
{

    Window::~Window()
    {
        glfwDestroyWindow(m_nativeWindow);
        RendererCommands::ShutdownGlfw();
    }

    void Window::Update()
    {
        CORE_PROFILE_FUNC();
        glfwPollEvents();
        {
            CORE_PROFILE_SCOPE("glfwSwapBuffers");
            glfwSwapBuffers(m_nativeWindow);
        }
    }

    void Window::SetWindowSize(int width, int height)
    {
        m_properties.width = width;
        m_properties.height = height;
    }

    void Window::Init(const Properties &props)
    {
        m_properties = props;
        RendererCommands::InitGlfw();

        m_nativeWindow = glfwCreateWindow(m_properties.width, m_properties.height, m_properties.title.c_str(), NULL, NULL);


        if (m_nativeWindow == NULL)
        {
            CORE_ASSERT(false, "Failed to create GLFW window");
            glfwTerminate();
        }

        glfwMakeContextCurrent(m_nativeWindow);

        if (props.vsync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        if (props.resizable)
            glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        else
            glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


        RendererCommands::InitGlew();

        // set callbacks
        {
        glfwSetWindowUserPointer(m_nativeWindow, this);

        glfwSetKeyCallback(m_nativeWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
            auto EventCallback = windowPtr->GetEventCallback();

            KeyCode myKey = KeyCode(key);
            KeyModifier myMod = KeyModifier(mods);

            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyPressedEvent e(myKey,myMod);
                EventCallback(e);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent e(myKey,myMod);
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

        glfwSetMouseButtonCallback(m_nativeWindow, [](GLFWwindow *window, int button, int action, int mods) {
            auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
            auto EventCallback = windowPtr->GetEventCallback();

            auto myButton = MouseButtonCode(button);
            KeyModifier myMod = KeyModifier(mods);
            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent e(myButton,myMod);
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

        glfwSetScrollCallback(m_nativeWindow, [](GLFWwindow *window, double xoffset, double yoffset) {
            auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
            auto EventCallback = windowPtr->GetEventCallback();

            MouseScrolledEvent e({xoffset, yoffset});
            EventCallback(e);
        });

        glfwSetCursorPosCallback(m_nativeWindow, [](GLFWwindow *window, double xpos, double ypos) {
            auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
            auto EventCallback = windowPtr->GetEventCallback();

            MouseMovedEvent e({xpos, ypos});
            EventCallback(e);
        });

        glfwSetWindowCloseCallback(m_nativeWindow, [](GLFWwindow *window) {
            auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
            auto EventCallback = windowPtr->GetEventCallback();

            WindowClosedEvent e;
            EventCallback(e);
        });

        glfwSetWindowFocusCallback(m_nativeWindow, [](GLFWwindow *window, int iconified) {
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

        glfwSetWindowSizeCallback(m_nativeWindow, [](GLFWwindow *window, int width, int height) {
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