#include "Editor.hpp"

void test();

#include <iostream>
int main()
{
    
    ant::Application::s_instance = new Editor::EditorApp();
    auto app = ant::Application::GetInstance();
    app->Init();
    app->Run();
    delete app;

    // test();

    return 0;
}