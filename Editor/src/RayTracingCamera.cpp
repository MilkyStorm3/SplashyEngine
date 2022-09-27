#include "RayTracingCamera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Sandbox
{

    void RayTracingCamera::CalculateProjection()
    {
        m_projection = glm::perspectiveFov(
            glm::radians(m_verticalFOV),
            (float)m_viewportWidth,
            (float)m_viewportHeight,
            m_nearClip,
            m_farClip);

        m_inverseProjection = glm::inverse(m_projection);        
    }

    void RayTracingCamera::CalculateView()
    {
        auto focusPoint = m_position + m_forwardDirection;
        m_view = glm::lookAt(m_position, focusPoint, m_upDirection);
        m_inverseView = glm::inverse(m_view);
    }

    void RayTracingCamera::CalculateRays()
    {
        m_rayDirections.resize(m_viewportHeight * m_viewportWidth);

        for (size_t y = 0; y < m_viewportHeight; y++)
        {
            for (size_t x = 0; x < m_viewportWidth; x++)
            {
                glm::vec2 coord = {float(x) / float(m_viewportWidth), float(y) / float(m_viewportHeight)};
                coord = coord * 2.f - 1.f;

                glm::vec4 target = m_inverseProjection * glm::vec4(coord, 1, 1); // maps pixel coordinates to camera coordinate system
                auto direction = m_inverseView * glm::vec4(
                                                     glm::normalize(glm::vec3(target) / target.w), // convert to non homogeneous 3d coordinate system and normalize to get directional vector
                                                     0);
                m_rayDirections[x + y * m_viewportWidth] = direction; // direction in world space
            }
        }
    }

}