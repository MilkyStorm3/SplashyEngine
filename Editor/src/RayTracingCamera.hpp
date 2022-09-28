#pragma once
#include <Core/Camera.hpp>
#include <vector>
#include <Core/Time.hpp>

namespace Sandbox
{

    class RayTracingCamera : public ant::Camera
    {
    public:
        RayTracingCamera() {}
        ~RayTracingCamera() {}

        void OnUpdate(ant::TimeStep ts);

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

        float m_movementSpeed = 0.7f;
        float m_mouseSpeed = 0.002f;

        bool m_resized = false;
        bool m_moved = false;
        bool m_rotated = false;
    private:
        glm::ivec2 m_mousePrev;
    };

} // namespace Sandbox
