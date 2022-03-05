#include "CommonInput.hpp"
#include <Core/Core.hpp>
#include <GLFW/glfw3.h> 

namespace ant::Common
{
    void *CommonInput::s_activeNativeWindow = nullptr;

    bool CommonInput::IsKeyPressed_IMPL(KeyCode key)
    {
        CORE_ASSERT(s_activeNativeWindow, "Active window must be set!");
        auto state = glfwGetKey((GLFWwindow*)s_activeNativeWindow, (int)key);

        switch (state)
        {
        case GLFW_PRESS:
            return true;

        case GLFW_REPEAT:
            return true;

        case GLFW_RELEASE:
            return false;

        default:
            break;
        }
        return false;
    }

    bool CommonInput::IsMouseButtonPressed_IMPL(MouseButtonCode buttonCode)
    {
        CORE_ASSERT(s_activeNativeWindow, "Active window must be set!");
        auto state = glfwGetMouseButton((GLFWwindow*)s_activeNativeWindow, (int)buttonCode);

        switch (state)
        {
        case GLFW_PRESS:
            return true;

        case GLFW_REPEAT:
            return true;

        case GLFW_RELEASE:
            return false;

        default:
            break;
        }
        return false;
    }

    glm::vec2 CommonInput::MousePos_IMPL()
    {
        CORE_ASSERT(s_activeNativeWindow, "Active window must be set!");
        double x, y;
        glfwGetCursorPos((GLFWwindow*)s_activeNativeWindow, &x, &y);
        return {x, y};
    }
}