#include "EditorCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Editor
{
    void EditorCamera::SetOrtographic(float p_size, float p_near, float p_far)
    {
        m_size = p_size;
        m_near = p_near;
        m_far = p_far;
        CalculateProjection();
    }

    void EditorCamera::CalculateProjection()
    {
        float left = -m_size * m_aspectRatio * 0.5f;
        float right = m_size * m_aspectRatio * 0.5f;
        float bottom = -m_size * 0.5f;
        float top = m_size * 0.5f;

        m_projection = glm::ortho(left, right, bottom, top, m_near, m_far);
    }
}