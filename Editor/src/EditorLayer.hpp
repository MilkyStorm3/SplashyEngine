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
#include "Sphere.hpp"

namespace Sandbox
{

    class EditorLayer : public ant::Layer
    {
    public:
        EditorLayer() {}
        ~EditorLayer() {}

        virtual void OnAttach() override;
        virtual void OnUpdate(ant::TimeStep ts) override;
        virtual void OnDraw() override {}
        virtual void OnDetach() override;
        virtual void OnEvent(ant::Event *event) override {}

    private:
        static glm::vec4 TraceRay(const Ray &ray, const Sphere &sphere, const RayTracingCamera &camera);
        void Render(const ImVec2 &viewport);
        void Clear(const glm::vec4 &color);

        static uint32_t Vec4ToPixel(const glm::vec4 &color);

        static void SphereImGuiPanel(const Sphere& sphere, size_t id);

    private:
        uint32_t *m_imageData = nullptr;
        ant::Ref<ant::Texture2D> m_imageTexture;

        bool m_viewportFocus = false;

        RayTracingCamera m_camera;
        std::vector<Sphere> m_spheres;
    };

}