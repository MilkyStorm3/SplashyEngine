#pragma once
#include <Core/Layer.hpp>
#include <Graphics/FrameBuffer.hpp>

#include <Graphics/Shader.hpp>

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
    };

}