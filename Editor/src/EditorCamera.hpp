#pragma once
#include <Core/Camera.hpp>

namespace Editor
{

    class EditorCamera
        : public ant::PerspectiveCamera
    {
    public:
        EditorCamera() {}
        ~EditorCamera() {}

        virtual void OnLogicUpdate(ant::TimeStep ts) override;

    public:
        float movementSpeed = 0.7f;
        float mouseSpeed = 0.002f;
        bool focused = true;

    private:
        glm::ivec2 m_mousePrev{0.f, 0.f};
        bool m_rotated = false;
    };

}
