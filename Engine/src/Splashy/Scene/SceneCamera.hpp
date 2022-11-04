#pragma once
#include "Core/Camera.hpp"

namespace ant
{

    class SceneCamera : public Camera
    {
    public:
        SceneCamera() {
            
        }
        ~SceneCamera() {}

        void SetOrtographic(float size, float near, float far);

        void SetOrtographicSize(float size)
        {
            m_size = size;
            CalculateProjection();
        }
        float GetOrtographicSize() const { return m_size; }

        void SetOrtographicNearClip(float n)
        {
            m_near = n;
            CalculateProjection();
        }
        float GetOrtographicNearClip() const { return m_near; }

        void SetOrtographicFarClip(float f)
        {
            m_far = f;
            CalculateProjection();
        }
        float GetOrtographicFarClip() const { return m_far; }

        void SetAspectRatio(float ratio)
        {
            m_aspectRatio = ratio;
            CalculateProjection();
        }

    private:
    void CalculateProjection();

    private:
        float m_size = 10.f;
        float m_near = -1.f, m_far = 1.f;

        float m_aspectRatio = 0.f;
    };

}
