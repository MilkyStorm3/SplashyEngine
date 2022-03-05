#include "SceneCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace ant
{
    void SceneCamera::SetOrtographic(float size, float near, float far)
    {
        m_size = size;
        m_near = near;
        m_far = far;

        CalculateProjection();
    }

    void SceneCamera::CalculateProjection()
    {
        float left = -m_size * m_aspectRatio * 0.5f;
        float right = m_size * m_aspectRatio * 0.5f;
        float bottom = -m_size * 0.5f;
        float top = m_size * 0.5f;

        m_projection = glm::ortho(left, right, bottom, top, m_near, m_far);
    }
}