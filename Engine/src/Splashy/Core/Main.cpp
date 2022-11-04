#include <Core/Application.hpp>

int main()
{
    auto app = ant::Application::GetInstance();
    app->Init();
    app->Run();
    delete app;

    return 0;
}