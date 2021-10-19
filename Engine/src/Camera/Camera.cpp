#include "Pch.h"
#include "Camera/Camera.hpp"
#include "Input/Event.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "debug/Instrumentation.hpp"

namespace ant
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.f, 1.f)), m_viewMatrix(1.f)
    {
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        m_boundaries.left = left;
        m_boundaries.right = right;
        m_boundaries.top = top;
        m_boundaries.bottom = bottom;
    }

    void OrthographicCamera::CalculateViewProjectionMatrix()
    {
        CORE_PROFILE_FUNC();
        glm::mat4 transformationMatrix = glm::translate(glm::mat4(1.f), m_translationVector) * glm::rotate(glm::mat4(1.f), m_rotation, glm::vec3(0.f, 0.f, 1.f));
        m_viewMatrix = glm::inverse(transformationMatrix);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

    void OrthographicCamera::SetRotation(float rotation)
    {
        m_rotation = rotation;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        CORE_PROFILE_FUNC();
        m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.f, 1.f);
        m_viewMatrix = glm::mat4(1.f);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

        m_boundaries.left = left;
        m_boundaries.right = right;
        m_boundaries.top = top;
        m_boundaries.bottom = bottom;
    }

    void OrthographicCamera::CalculateProjection()
    {
        CORE_PROFILE_FUNC();
        m_projectionMatrix = glm::ortho(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom, -1.f, 1.f);
        m_viewMatrix = glm::mat4(1.f);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

        m_boundaries.left = -m_aspectRatio * m_zoom;
        m_boundaries.right = m_aspectRatio * m_zoom;
        m_boundaries.top = -m_zoom;
        m_boundaries.bottom = m_zoom;
    }

    void OrthographicCameraController::OnWindowResized(WindowRezisedEvent *e)
    {
        auto size = e->GetWindowSize();
        OnResize(size.x, size.y);
    }

    void OrthographicCameraController::OnResize(float x, float y)
    {
        float ratio = x / y;

        m_camera->SetAscpectRatio(ratio);
    }

    void OrthographicCamera::SetPosition(const glm::vec3 &position)
    {
        m_translationVector = position;
    }

} // namespace ant
