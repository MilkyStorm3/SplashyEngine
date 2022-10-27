#include "Application.hpp"
#include "Logger.hpp"
#include "Core/Core.hpp"
#include "Render/RendererCommands.hpp"
#include "Eventing/InputEvents.hpp"
#include "Input/Input.hpp"
#include "ImGuiLayer.hpp"
#include "Utilities/InstrumentationMacros.hpp"
#include <Eventing/Eventing.hpp>

void test();

namespace ant
{
    SPL_DEF Application *Application::s_instance;

    Application::~Application()
    {
        RendererCommands::Shutdown();
    }

    void Application::Init()
    {
        Instrumentor::InitSession("CORE", InstrumentationLevel::General);
        Logger::Init();
        Input::Init();

        CORE_TRACE("Hello!");

        m_window = Window::Create();

        EventEmitter::SetMainEventCallback(CORE_BIND_EVENT_FN(this, Application::OnEvent));

        m_window->SetEventCallback(EventEmitter::GetMainCallback());

        m_window->Init(m_appdata.windowSettings.title, m_appdata.windowSettings.width, m_appdata.windowSettings.height, true);

        RendererCommands::Init(); //?call when context is initialized

        m_layerStack.PushOverlay(MakeRef<ImGuiLayer>());

        m_appInitFn();
    }

    void Application::Run()
    {
        static TimePoint lastFrameTime = Time::Now();

        while (m_appdata.running)
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