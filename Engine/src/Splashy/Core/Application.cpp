#include "Application.hpp"
#include "Logger.hpp"
#include "Core/Core.hpp"
#include "Render/RendererCommands.hpp"
#include "Eventing/InputEvents.hpp"
#include "Input/Input.hpp"
#include "ImGuiLayer.hpp"
#include "Utilities/InstrumentationMacros.hpp"
#include <Eventing/Eventing.hpp>

extern ant::Application *CreateApplication();

namespace ant
{
    SPL_DEF Application *Application::s_instance = CreateApplication();

    Application::~Application()
    {
        RendererCommands::Shutdown();
    }

    void Application::Init()
    {
        CORE_ASSERT(m_configuration.windowSettings.title != "Untitled", "Configure application settings in client's constructor");
        Instrumentor::InitSession("CORE", InstrumentationLevel::General);
        Logger::Init();
        Input::Init();

        CORE_TRACE("Hello!");

        m_window = Window::Create();

        EventEmitter::SetMainEventCallback(CORE_BIND_EVENT_FN(this, Application::HandleEvent));

        m_window->SetEventCallback(EventEmitter::GetMainCallback());

        m_window->Init(m_configuration.windowSettings.title, m_configuration.windowSettings.width, m_configuration.windowSettings.height, true, false);

        RendererCommands::Init(); //?call when context is initialized

        m_layerStack.PushOverlay(MakeRef<ImGuiLayer>());

        OnInit();
    }

    void Application::Run()
    {
        m_running = true;
        static TimePoint lastFrameTime = Time::Now();

        while (m_running)
        {
            TimePoint time = Time::Now();
            TimeStep frameTime = time - lastFrameTime;
            lastFrameTime = time;

            m_layerStack.OnUpdate(frameTime);
            RendererCommands::Clear();

            m_layerStack.OnDraw();
            m_window->Update();
        }
    }

    void Application::HandleEvent(Event &e)
    {
        // CORE_INFO("Event triggered {0}", e.GetStringLog());
        OnEvent(e);

        m_layerStack.OnEvent(&e);

        EventDispatcher d(e);

        d.DispatchEvent<WindowClosedEvent>(
            [this](WindowClosedEvent &wce) -> bool
            {
                m_running = false;
                return true;
            });
    }

}