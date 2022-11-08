#pragma once
#include <Core/Layer.hpp>
#include <Graphics/FrameBuffer.hpp>

#include <Graphics/Shader.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/UniformBuffer.hpp>
#include <Graphics/VertexArray.hpp>
#include "EditorCamera.hpp"

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
        ant::Ref<ant::FrameBuffer> m_framebuffer;
        ant::Ref<ant::Shader> m_shader;
        ant::Ref<ant::VertexArray> m_vertexArray;
        ant::Ref<ant::UniformBuffer> m_projectionBuffer;
        ant::Ref<ant::UniformBuffer> m_cubeTransformBuffer;
        ant::Ref<EditorCamera> m_camera;
    };

}