/* #pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace ant
{
    class TransformComponent
    {
    public:
        TransformComponent()
            : m_trs(1.f), m_rotation(0.f), m_scale(1.f), m_translation(0.f,0.f, 0.f){}

        virtual ~TransformComponent() {}

        void CalculateTranformationMatrix();
        const glm::mat4 &GetTransformationMatrix() const { return m_trs; }

        inline  glm::vec2 GetScale() { return m_scale; }
        inline float GetRotation() { return m_rotation; }
        inline const glm::vec3 &GetPosition() { return m_translation; }

        inline void SetScale(const glm::vec2& scale) { m_scale = scale; }
        inline void SetRotation(float rotation) { m_rotation = rotation; }
        inline void SetPosition(const glm::vec3 &trans) { m_translation = trans; }

    private:
        glm::vec3 m_translation;
        float m_rotation;
        glm::vec2 m_scale;
        glm::mat4 m_trs;
    };

} // namespace ant */