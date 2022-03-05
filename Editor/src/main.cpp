#include "Editor.hpp"

int main()
{
    
    ant::Application::s_instance = new Editor::EditorApp();
    auto app = ant::Application::GetInstance();
    app->Init();
    app->Run();
    delete app;

    return 0;
}