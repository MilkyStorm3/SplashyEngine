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

        inline const glm::mat4 &GetProjection() { return m_projection; }

    protected:
        glm::mat4 m_projection = glm::mat4(1.f);
    };

} // namespace ant
