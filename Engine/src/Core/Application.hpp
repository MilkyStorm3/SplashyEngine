#pragma once
#include "Graphics/Window.hpp"
#include "Layer.hpp"
namespace ant
{
    class Event;

    struct AppSettings
    {
        bool running = true;

        struct //? window properties
        {
            uint32_t width = 1240;
            uint32_t height = 720;
            const char *title = "LearnOpenGL";
        } windowSettings;
    };

    class Application
    {

    public:
        ~Application();

        void Init();
        void Run();

        void OnEvent(Event &e);

        const Window &GetWindow() const { return m_window; }

        static Application *GetInstance() { return s_instance; }

        static Application *s_instance;

    protected:
        Application() {}

    private:
        AppSettings m_appdata;

        Window m_window;

    protected:
        LayerStack m_layerStack;
        std::function<void()> m_appInitFn;
    };

}