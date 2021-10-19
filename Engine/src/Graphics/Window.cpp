#include "Pch.h"
#include "Graphics/Window.hpp"

#include <Gl.h>
#include <stb_image.h>
#include "debug/Instrumentation.hpp"
#include "Input/GlErrorHandler.hpp"

namespace df
{
    bool Window::GlLibsWrangler::s_glProfileSet = false;
    bool Window::GlLibsWrangler::s_gladLoaded = false;

    void Window::GlLibsWrangler::InitGlfw()
    {
        if (!s_glProfileSet)
        {
            CORE_ASSERT(glfwInit(), "Failed to initialize GLFW ");
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }
        stbi_set_flip_vertically_on_load(true);
        glewInit();
    }

    void Window::GlLibsWrangler::InitGlad()
    {
        // CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize OpenGL context");
        // CORE_ASSERT(glewInit(), "Failed to initialize OpenGL context");
        CORE_INFO("Started OpenGL {0}", glGetString(GL_VERSION));
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearDepth(1.f);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(&GlErrorHandler::GlErrorCallbackFunction, NULL);
    }

    void Window::GlLibsWrangler::DeactivateGlfw()
    {
        glfwTerminate();
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_nativeWindow);
        m_glLibsWrangler.DeactivateGlfw(); //move it from here
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
        m_glLibsWrangler.InitGlfw();

        m_nativeWindow = glfwCreateWindow(m_properties.width, m_properties.height, m_properties.title.c_str(), NULL, NULL);

        if (m_nativeWindow == NULL)
        {
            CORE_ERROR("Failed to create GLFW window");
            glfwTerminate();
            std::raise(SIGINT);
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

        m_glLibsWrangler.InitGlad();

        // set callbacks
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
                EventCallback(&e);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent e(myKey,myMod);
                EventCallback(&e);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent e(myKey, myMod, true);
                EventCallback(&e);
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
                EventCallback(&e);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent e(myButton, myMod);
                EventCallback(&e);
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
            EventCallback(&e);
        });

        glfwSetCursorPosCallback(m_nativeWindow, [](GLFWwindow *window, double xpos, double ypos) {
            auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
            auto EventCallback = windowPtr->GetEventCallback();

            MouseMovedEvent e({xpos, ypos});
            EventCallback(&e);
        });

        glfwSetWindowCloseCallback(m_nativeWindow, [](GLFWwindow *window) {
            auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
            auto EventCallback = windowPtr->GetEventCallback();

            WindowClosedEvent e;
            EventCallback(&e);
        });

        glfwSetWindowFocusCallback(m_nativeWindow, [](GLFWwindow *window, int iconified) {
            auto *windowPtr = (Window *)glfwGetWindowUserPointer(window);
            auto EventCallback = windowPtr->GetEventCallback();

            switch (iconified)
            {
            case GLFW_TRUE:
            {
                WindowFocusedEvent e;
                EventCallback(&e);
                break;
            }
            case GLFW_FALSE:
            {
                WindowUnfocusedEvent e;
                EventCallback(&e);
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
            EventCallback(&e);
        });
    }

} // namespace df