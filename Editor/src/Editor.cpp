#pragma once
#include <Core/Application.hpp>
#include <Eventing/Eventing.hpp>
#include <Eventing/InputEvents.hpp>
#include "EditorLayer.hpp"

namespace Sandbox
{

    class SandoxApp : public ant::Application
    {
    public:
        SandoxApp()
        {
            m_configuration.renderApi = ant::RenderApi::OpenGl;
            m_configuration.windowSettings.title = "Editor";
        }
        ~SandoxApp()
        {
        }

        void OnInit() override
        {
            m_layerStack.PushLayer(ant::MakeRef<EditorLayer>());
        }

        void OnEvent(ant::Event &e) override
        {
        }

    private:
    };

}

ant::Application *CreateApplication()
{
    return new Sandbox::SandoxApp();
}
