#pragma once
#include <glm/glm.hpp>

namespace Sandbox
{
    struct Ray
    {
        glm::vec3 direction;
        glm::vec3 origin;
    };

    struct Sphere
    {
        float radius = 0.5f;
        glm::vec3 origin{0.f, 0.f, 0.f};
        glm::vec3 color{1.f, 1.f, 1.f};
    };

    struct Scene
    {
        std::vector<Sphere> spheres;
    };

} // namespace
