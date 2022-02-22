#include "Input/Input.hpp"

#include "Pch.h"
#include <Gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Utilities/InstrumentationMacros.hpp"

namespace ant
{

    Ref<Window> Input::s_activeWindow = nullptr;

    void Input::UseWindow(Ref<Window> window)
    {
        s_activeWindow = window;
    }

    glm::vec2 Input::MousePos()
    {
        double x, y;
        glfwGetCursorPos((GLFWwindow *)s_activeWindow->GetNativeWindow(), &x, &y);
        return {x, y};
    }

    bool Input::IsKeyPressed(KeyCode key)
    {
        auto state = glfwGetKey((GLFWwindow *)s_activeWindow->GetNativeWindow(), (int)key);

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

    bool Input::IsMouseButtonPressed(MouseButtonCode buttonCode)
    {
        auto state = glfwGetMouseButton((GLFWwindow *)s_activeWindow->GetNativeWindow(), (int)buttonCode);

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

} // namespace ant