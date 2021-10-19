#pragma once



#ifdef PARTICLES

#include "Core/Core.hpp"
#include <glm/glm.hpp>
#include "Render/Primitive.hpp"
#include "Core/Time.hpp"

namespace df
{

    struct ParticleProps
    {
        glm::vec2 position;
        glm::vec2 velocity, velocityVariation;
        glm::vec2 size;
        glm::vec4 birthColor, deathColor;
        float lifeTime;
        float rotation, rotationVelocity, rotationVelocityVariation;
    };

    class Particle
        : public Quad
    {
        friend class ParticleSystem;

    public:
        Particle() {}
        Particle(const ParticleProps &props);
        ~Particle() {}

        void OnUpdate();
        void SetProps(const ParticleProps &props);
        ParticleProps &GetProps() { return m_props; }

    private:
        ParticleProps m_props;
        bool m_active = false;
        float m_lifeRemaining;
    };

    class ParticleSystem
    {
    public:
        ParticleSystem();
        ~ParticleSystem() {}

        void OnUpdate();
        void OnDraw();

        void Emit(const ParticleProps &props);

    private:
        static constexpr size_t CountLimit = 1000;
        size_t m_emitIndex = 0;
        std::vector<Particle> m_particles;
    };

}

#endif