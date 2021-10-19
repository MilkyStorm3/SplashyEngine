#include "Application.hpp"
#include "Logger.hpp"

void test();
namespace ant
{

    void Application::Init()
    {
        ant::Logger::Init();

        CORE_INFO("Hello!");
    }

    void Application::Run()
    {

        test();
        //  while(m_appdata.running){}        
    }

}