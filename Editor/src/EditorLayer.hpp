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
        void Render(const ImVec2 &viewport);
        void Clear(const glm::vec4 &color);

        static uint32_t Vec4ToPixel(const glm::vec4 &color);

        struct RayTracerPayload
        {
            glm::vec3 hitPoint;
            glm::vec3 surfaceNormal;
            uint32_t hitObjectIndex;
        };

        glm::vec4 PerPixel(size_t index);
        RayTracerPayload TraceRay(const Ray &ray);
        RayTracerPayload ClosestHit(const Ray &ray, float hitDistance, size_t sphereIndex);
        RayTracerPayload Miss();

    private:
        uint32_t *m_imageData = nullptr;
        ant::Ref<ant::Texture2D> m_imageTexture;

        bool m_viewportFocus = false;

        RayTracingCamera m_camera;
        glm::vec3 m_lightDirection{-1, -1, -1};
        glm::vec4 m_clearColor{74.f / 255.f, 70.f / 255.f, 166.f / 255.f, 1};
        Scene m_scene;
    };

}