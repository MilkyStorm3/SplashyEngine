#include "Editor.hpp"

int main()
{
    
    ant::Application::s_instance = new Sandbox::SandboxApp();
    auto app = ant::Application::GetInstance();
    app->Init();
    app->Run();
    delete app;

    return 0;
}