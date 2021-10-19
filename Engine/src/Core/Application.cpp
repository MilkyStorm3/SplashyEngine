#include "Application.hpp"
#include "Logger.hpp"
#include <Gl.h>
#include "Core/Core.hpp"
#include "Render/RendererCommands.hpp"
#include "Input/Event.hpp"
void test();
namespace ant
{
    Application::~Application()
    {
        //todo shutdown glew
    }

    void Application::Init()
    {
        ant::Logger::Init();

        CORE_INFO("Hello!");
       

        m_window.SetEventCallback(CORE_BIND_EVENT_FN(Application::OnEvent));
        
        m_window.Init({m_appdata.windowSettings.width,
           m_appdata.windowSettings.height,
           m_appdata.windowSettings.title,
           true, false});
    }

    void Application::Run()
    {

        // test();
         while(m_appdata.running){
             m_window.Update();
         }
    }
    
    void Application::OnEvent(Event& e) 
    {
        CORE_INFO("Event triggered {0}",e.GetStringLog());

        EventDispatcher d(e);

        d.DispatchEvent<WindowClosedEvent>([this](WindowClosedEvent& wce)->bool{
            m_appdata.running = false;
            return true;
        });
    }

}