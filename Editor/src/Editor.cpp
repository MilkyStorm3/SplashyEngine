#pragma once
#include <Core/Application.hpp>
#include <Eventing/Eventing.hpp>
#include <Eventing/InputEvents.hpp>
#include "EditorLayer.hpp"

namespace Editor
{

    class EditorApp : public ant::Application
    {
    public:
        EditorApp()
        {
            m_configuration.renderApi = ant::RenderApi::OpenGl;
            m_configuration.windowSettings.title = "Editor";
        }
        ~EditorApp()
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
    return new Editor::EditorApp();
}
