#include "Pch.h"
#include "Input/Input.hpp"
#include <Gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Utilities/InstrumentationMacros.hpp"
namespace ant
{

    Window *Input::s_activeWindow = nullptr;

    void Input::UseWindow(Window *window)
    {
        s_activeWindow = window;
    }

    glm::vec2 Input::MousePos()
    {
        double x, y;
        glfwGetCursorPos(s_activeWindow->m_nativeWindow, &x, &y);
        return {x, y};
    }

    glm::vec2 Input::MouseWorldPos(const OrthographicCamera &camera)
    {
        auto mpos = MousePos();
        auto x = mpos.x, y = mpos.y;

        auto width = s_activeWindow->GetSize().x;
        auto height = s_activeWindow->GetSize().y;

        auto bounds = camera.GetBoundaries();

        auto pos = camera.GetPosition();

        x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
        y =  (y / height) * bounds.GetHeight() - bounds.GetHeight() * 0.5f ;

        glm::vec4 beforeRotation = {x + pos.x, y + pos.y, 0.0f, 1.f};

        auto rotate = glm::rotate(glm::mat4(1.f),camera.GetRotation(),{0.f,0.f,1.f});
        auto out =  rotate*beforeRotation;    

        return {out.x, out.y};
    }

    bool Input::IsKeyPressed(KeyCode key)
    {
        auto state = glfwGetKey(s_activeWindow->m_nativeWindow, (int)key);

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
        auto state = glfwGetMouseButton(s_activeWindow->m_nativeWindow, (int)buttonCode);

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