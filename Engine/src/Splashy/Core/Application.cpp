#include "Application.hpp"
#include "Logger.hpp"
#include "Core/Core.hpp"
#include "Render/RendererCommands.hpp"
#include "Input/Event.hpp"
#include "ImGuiLayer.hpp"

void test();

namespace ant
{
    Application *Application::s_instance;

    Application::~Application()
    {
        RendererCommands::Shutdown();
    }

    void Application::Init()
    {
        ant::Logger::Init();

        CORE_TRACE("Hello!");

        m_window.SetEventCallback(CORE_BIND_EVENT_FN(this, Application::OnEvent));

        m_window.Init({m_appdata.windowSettings.width,
                       m_appdata.windowSettings.height,
                       m_appdata.windowSettings.title,
                       true, false});

        m_layerStack.PushOverlay(MakeRef<ImGuiLayer>());

        m_appInitFn();
    }

    void Application::Run()
    {

        while (m_appdata.running)
        {
            m_layerStack.OnUpdate();
            RendererCommands::Clear();

            m_layerStack.OnDraw();
            m_window.Update();
        }
    }

    void Application::OnEvent(Event &e)
    {
        // CORE_INFO("Event triggered {0}", e.GetStringLog());

        m_layerStack.OnEvent(&e);

        EventDispatcher d(e);

        d.DispatchEvent<WindowClosedEvent>(
            [this](WindowClosedEvent &wce) -> bool
            {
                m_appdata.running = false;
                return true;
            });
    }

}