#pragma once
#include <Core/Application.hpp>
#include "EditorLayer.hpp"

namespace Sandbox
{

    class SandboxApp : public ant::Application
    {
    public:
        SandboxApp(){

            m_appInitFn = [this](){
                m_layerStack.PushLayer(ant::MakeRef<EditorLayer>());
            };

        }
        ~SandboxApp() {}

    private:

    };

}
