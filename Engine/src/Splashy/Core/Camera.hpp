#pragma once
#include <glm/glm.hpp>
#include "Core/Core.hpp"
#include "Core/Time.hpp"

namespace ant
{

    class Camera
    {
        friend class PerspectiveCamera;
        friend class OrtographicCamera;

    public:
        Camera() {}
        Camera(const glm::mat4 &projection) : m_projection(projection) {}
        ~Camera() {}

        void OnUpdate(ant::TimeStep ts);

        inline const glm::mat4 &GetCameraMatrix() { return m_projectionView; }

        void SetFarClip(float p_far);
        void SetNearClip(float p_near);

        inline float GetNearClip() { return m_nearClip; }
        inline float GetFarClip() { return m_farClip; }

    protected:
        virtual void OnLogicUpdate(ant::TimeStep ts) = 0;

    protected:
        bool m_reView = true;
        bool m_reProject = true;

    private:
        virtual void CalculateProjection() = 0;
        virtual void CalculateView() = 0;

    private:
        glm::mat4 m_projection = glm::mat4(1.f);
        glm::mat4 m_view = glm::mat4(1.f);
        glm::mat4 m_projectionView = glm::mat4(1.f);

        float m_nearClip = 0.1f, m_farClip = 100.f;
    };

    class PerspectiveCamera
        : public Camera
    {
    public:
        PerspectiveCamera() {}
        ~PerspectiveCamera() {}

        void Resize(const glm::vec2 &viewport);

        void SetPosition(const glm::vec3 &position);
        void SetProjection(float vFov, float nearClip, float farClip);

        inline const glm::vec3 &GetPosition() const { return m_position; }

    private:
        virtual void CalculateProjection() override;
        virtual void CalculateView() override;

    protected:
        glm::vec3 m_position{0.f, 0.f, 3.f};
        glm::vec3 m_forwardDirection{0.f, 0.f, -1.f};
        glm::vec3 m_upDirection{0.f, 1.f, 0.f};

        float m_verticalFOV = 45.f;
        glm::vec2 m_viewportDims{0.f, 0.f};
    };

    class OrtographicCamera : public ant::Camera
    {
    public:
        OrtographicCamera() {}
        ~OrtographicCamera() {}

        void SetOrtographic(float size, float near, float far);
        void SetOrtographicSize(float size);

        float GetOrtographicSize() const { return m_size; }
        float GetOrtographicNearClip() const { return m_near; }
        float GetOrtographicFarClip() const { return m_far; }

        void SetAspectRatio(float ratio);

    private:
        virtual void CalculateProjection() override;
        virtual void CalculateView() override;

    private:
        float m_size = 10.f;
        float m_near = -1.f, m_far = 1.f;
        float m_aspectRatio = 0.f;
    };

} // namespace ant
