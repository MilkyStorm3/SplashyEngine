#include "Pch.h"
#ifdef PARTICLES
#include "Render/Particles.hpp"
#include "Render/Renderer.hpp"
#include "Core/Random.hpp"
#include "debug/Instrumentation.hpp"
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace df
{
    Particle::Particle(const ParticleProps &props)
        : m_props(props)
    {
        m_lifeRemaining = m_props.lifeTime;
    }

    void Particle::OnUpdate()
    {
        CORE_PROFILE_FUNC();
        if (!m_active)
            return;

        m_lifeRemaining -= TimeStep::GetFrameTime().Seconds();

        if (m_lifeRemaining <= 0)
        {
            m_active = false;
            return;
        }

        float life = m_lifeRemaining / m_props.lifeTime;
        SetColor(glm::lerp(m_props.deathColor, m_props.birthColor, life));
        SetScale(glm::lerp(glm::vec2(0.f), m_props.size, life));

        glm::vec2 vel = {
            m_props.velocity.x * m_props.velocityVariation.x * (Random::Float() - 0.5f),
            m_props.velocity.y * m_props.velocityVariation.y * (Random::Float() - 0.5f)};

        m_props.velocity = vel;

        m_props.position += vel * TimeStep::GetFrameTime().Seconds();
        SetPosition({m_props.position, 1.f});

        float rot = m_props.rotationVelocity * m_props.rotationVelocityVariation * (Random::Float() - 0.5f);
        m_props.rotation += rot * TimeStep::GetFrameTime().Seconds();
        // m_props.rotation = rot; //! another way of animating
        SetRotation(glm::radians(m_props.rotation));
    }

    void Particle::SetProps(const ParticleProps &props)
    {
        m_props = props;
        m_lifeRemaining = m_props.lifeTime;
    }

    ParticleSystem::ParticleSystem()
    {
        m_particles.resize(CountLimit);
    }

    void ParticleSystem::OnUpdate()
    {
        CORE_PROFILE_FUNC();
        for (auto &&i : m_particles)
            i.OnUpdate();
    }

    void ParticleSystem::OnDraw()
    {
        CORE_PROFILE_FUNC();
        for (auto &&i : m_particles)
        {
            if (!i.m_active)
                continue;
            Renderer2D::DrawShape(i);
        }
    }

    void ParticleSystem::Emit(const ParticleProps &props)
    {
        CORE_PROFILE_FUNC();
        m_particles[m_emitIndex].SetProps(props);
        m_particles[m_emitIndex].m_active = true;

        m_emitIndex = ++m_emitIndex % (CountLimit - 1);
    }

}

#endif