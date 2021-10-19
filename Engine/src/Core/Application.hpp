#pragma once
#include "Graphics/Window.hpp"

namespace ant
{
    class Event;
    struct AppSettings
    {
        struct
        {
            uint32_t width = 1240;
            uint32_t height = 720;

        } InitialWindowSize;

        const char *title = "LearnOpenGL";
    };

    class Application
    {

    public:
        ~Application();

        virtual void Init();
        virtual void Run();

        void OnEvent(Event& e);


    protected:
        Application() {}

    private:
        struct
        {
            bool running = true;

            struct
            {
                uint32_t width = 1240;
                uint32_t height = 720;
                const char *title = "LearnOpenGL";
            } windowSettings;

        } m_appdata;
        Window m_window;
    };

}