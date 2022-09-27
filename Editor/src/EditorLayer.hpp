#pragma once
#include <Core/Layer.hpp>
#include <Graphics/FrameBuffer.hpp>

#include <Graphics/Shader.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/UniformBuffer.hpp>
#include <Graphics/VertexArray.hpp>
#include <Scene/SceneCamera.hpp>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include "RayTracingCamera.hpp"

#include <imgui.h>

namespace Sandbox
{
    struct Ray
    {
        glm::vec3 direction;
    };

    class EditorLayer : public ant::Layer
    {
    public:
        EditorLayer() {}
        ~EditorLayer() {}

        virtual void OnAttach() override;
        virtual void OnUpdate(ant::TimeStep ts) override;
        virtual void OnDraw() override;
        virtual void OnDetach() override;
        virtual void OnEvent(ant::Event *event) override;

    private:
        glm::vec4 PerPixel(const Ray &ray);
        void Render(const ImVec2 &viewport);

    private:
        uint32_t *m_imageData = nullptr;
        ant::Ref<ant::Texture2D> m_imageTexture;

        glm::vec3 m_sphereColor = {1.f, 1.f, 0.f};
        float m_sphereRadius = 0.5f;

        glm::vec3 m_lightDirection = {-1.f, -1.f, -1.f};
        RayTracingCamera m_camera;
    };

}