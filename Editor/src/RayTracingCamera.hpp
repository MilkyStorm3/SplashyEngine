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
        void Resize(const glm::vec2 &viewport);

        void SetProjection(float vFov, float nearClip, float farClip);
        void SetPosition(const glm::vec3 &position);

        inline const glm::vec3 &GetPosition() const { return m_position; }
        inline const std::vector<glm::vec3> &GetRayDirections() const { return m_rayDirections; }
        inline const glm::vec3 &GetRayDirection(size_t index) const { return m_rayDirections.at(index); }

    public:
        float movementSpeed = 0.7f;
        float mouseSpeed = 0.002f;

    private:
        void CalculateProjection();
        void CalculateView();
        void CalculateRays();

    private:
        float m_verticalFOV;
        float m_nearClip;
        float m_farClip;

        bool m_moved = false;
        bool m_resized = false;
        bool m_rotated = false;

        glm::ivec2 m_mousePrev;
        glm::vec2 m_viewportDims;

        glm::vec3 m_position{0.f, 0.f, 3.f};
        glm::vec3 m_forwardDirection{0.f, 0.f, -1.f};
        glm::vec3 m_upDirection{0.f, 1.f, 0.f};

        glm::mat4 m_view{1.f};
        glm::mat4 m_inverseView{1.f};
        glm::mat4 m_inverseProjection{1.f};

        std::vector<glm::vec3> m_rayDirections;
    };

} // namespace Sandbox
