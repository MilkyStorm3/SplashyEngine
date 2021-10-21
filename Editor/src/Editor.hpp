#pragma once
#include <Core/Application.hpp>
#include "EditorLayer.hpp"

namespace Editor
{

    class EditorApp : public ant::Application
    {
    public:
        EditorApp(){

            m_appInitFn = [this](){
                m_layerStack.PushLayer(ant::MakeRef<EditorLayer>());
            };

        }
        ~EditorApp() {}

    private:

    };

}
