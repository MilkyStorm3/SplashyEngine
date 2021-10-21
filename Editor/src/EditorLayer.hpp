#pragma once
#include <Input/Event.hpp>
#include <Core/Layer.hpp>
#include <Graphics/FrameBuffer.hpp>
#include <Camera/Camera.hpp>
namespace Editor
{

    class EditorLayer : public ant::Layer
    {
    public:
        EditorLayer() {}
        ~EditorLayer() {}

        virtual void OnAttach() override;
        virtual void OnUpdate() override;
        virtual void OnDraw() override;
        virtual void OnDetach() override;
        virtual void OnEvent(ant::Event *event) override;

    private:
        void DockSpace();

        // ant::Ref<ant::FrameBuffer> m_framebuffer;
        // ant::Ref<ant::OrthographicCameraController> m_camera;
    };

}