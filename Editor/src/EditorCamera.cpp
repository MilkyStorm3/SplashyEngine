#include "EditorCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <Input/Input.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Editor
{
    void EditorCamera::OnLogicUpdate(ant::TimeStep ts)
    {

        if (!focused)
        {
            return;
        }

        glm::ivec2 mouseCords = ant::Input::MousePos();
        glm::ivec2 mouseDelta = m_mousePrev - mouseCords;
        m_mousePrev = mouseCords;

        float t = ts.Seconds();
        glm::vec3 rightDirection = glm::cross(m_forwardDirection, m_upDirection);

        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_W))
        {
            m_position += m_forwardDirection * movementSpeed * t;
            m_reView = true;
        }
        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_S))
        {
            m_position -= m_forwardDirection * movementSpeed * t;
            m_reView = true;
        }
        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_D))
        {
            m_position += rightDirection * movementSpeed * t;
            m_reView = true;
        }
        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_A))
        {
            m_position -= rightDirection * movementSpeed * t;
            m_reView = true;
        }
        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_SPACE))
        {
            m_position += m_upDirection * movementSpeed * t;
            m_reView = true;
        }
        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_LEFT_SHIFT))
        {
            m_position -= m_upDirection * movementSpeed * t;
            m_reView = true;
        }

        // camera rotation

        if (m_rotated && !ant::Input::IsKeyPressed(ant::KeyCode::KEY_LEFT_CONTROL))
        {
            ant::Input::SetCursor(ant::CursorStyle::Normal);
            m_rotated = false;
        }

        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_LEFT_CONTROL) && (mouseDelta.x != 0.f || mouseDelta.y != 0.f))
        {
            ant::Input::SetCursor(ant::CursorStyle::Disabled);

            float yawDelta = mouseDelta.x * mouseSpeed;
            float pitchDelta = mouseDelta.y * mouseSpeed;

            glm::quat rotationQ = glm::normalize(

                glm::cross(
                    glm::angleAxis(pitchDelta, rightDirection),
                    glm::angleAxis(yawDelta, m_upDirection))

            );

            m_forwardDirection = glm::rotate(rotationQ, m_forwardDirection);
            m_rotated = m_reView = true;
        }
    }

}