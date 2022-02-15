#include "Pch.h"
#include "Render/Transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ant
{
    void TransformComponent::CalculateTranformationMatrix()
    {
        CORE_PROFILE_FUNC();
        m_trs = glm::translate(glm::mat4(1.f), m_translation) *
                glm::rotate(glm::mat4(1.f),-m_rotation, glm::vec3(0.f, 0.f, 1.f)) *
                glm::scale(glm::mat4(1.f), glm::vec3(m_scale.x, m_scale.y, 1.f));
    }
  
} // namespace ant