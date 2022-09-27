#pragma once
#include <Core/Camera.hpp>
#include <vector>

namespace Sandbox
{

    class RayTracingCamera : public ant::Camera
    {
    public:
        // RayTracingCamera(float verticalFov, float nearClip, float farClip, uint32_t width, uint32_t height);
        RayTracingCamera() {}
        ~RayTracingCamera() {}

        // private:
        void CalculateProjection();
        void CalculateView();
        void CalculateRays();

        // private:
        glm::mat4 m_view{1.f};
        glm::mat4 m_inverseView{1.f};
        glm::mat4 m_inverseProjection{1.f};

        float m_verticalFOV;
        float m_nearClip;
        float m_farClip;

        glm::vec3 m_position{0.f, 0.f, 3.f};
        glm::vec3 m_forwardDirection{0.f, 0.f, -1.f};
        glm::vec3 m_upDirection{0.f, 1.f, 0.f};

        uint32_t m_viewportWidth, m_viewportHeight;

        std::vector<glm::vec3> m_rayDirections;
    };

} // namespace Sandbox
