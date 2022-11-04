#pragma once
#include <glm/glm.hpp>
#include "Core/Core.hpp"

namespace ant
{

    class Camera
    {
    public:
        Camera() {}
        Camera(const glm::mat4 &projection) : m_projection(projection) {}

        virtual ~Camera() {}

        virtual void OnUpdate(ant::TimeStep ts)
        {
            PerUpdate(ts);
        }

        virtual void PerUpdate(ant::TimeStep ts) = 0;

        inline const glm::mat4 &GetProjection() { return m_projection; }

    protected:
        glm::mat4 m_projection = glm::mat4(1.f);
    };

    class PerspectiveCamera
        : public Camera
    {
    public:
        PerspectiveCamera() {}
        ~PerspectiveCamera() {}

        void SetProjection(float vFov, float nearClip, float farClip);
        void SetPosition(const glm::vec3 &position);
        void Resize(const glm::vec2 &viewport);

        inline const glm::vec3 &GetPosition() const { return m_position; }

        void PerUpdate(ant::TimeStep ts) override;

    protected:
        void CalculateProjection();
        void CalculateView();

    protected:
        glm::vec3 m_position{0.f, 0.f, 3.f};
        glm::vec3 m_forwardDirection{0.f, 0.f, -1.f};
        glm::vec3 m_upDirection{0.f, 1.f, 0.f};

        glm::mat4 m_view{1.f};

        bool m_reView = true;
        bool m_reProject = true;

        float m_nearClip, m_farClip, m_verticalFOV;
        glm::vec2 m_viewportDims;
    };

} // namespace ant
