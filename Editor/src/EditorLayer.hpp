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

namespace Editor
{

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
        uint32_t *m_imageData = nullptr;
        ant::Ref<ant::Texture2D> m_imageTexture;

        glm::vec4 PerPixel(const glm::vec2 &uv);
    };

}