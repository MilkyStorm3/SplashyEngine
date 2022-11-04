#pragma once
#include "Graphics/Window.hpp"
#include "Layer.hpp"
#include "Render/RendererCommands.hpp"
#include "Core/Time.hpp"

namespace ant
{
    class Event;

    struct AppSettings
    {

        struct //? window properties
        {
            uint32_t width = 1240;
            uint32_t height = 720;
            const char *title = "Untitled";
        } windowSettings;

        RenderApi renderApi = RenderApi::OpenGl;
    };

    class Application
    {
    public:
        virtual ~Application();

        void Init();
        void Run();

        void Terminate() { m_running = false; }

        /* Use for pushing layer etc. configuration has to be done in the constructor */
        virtual void OnInit() = 0;

        /* Gets called on every single event before any other handler */
        virtual void OnEvent(Event &e) = 0;

        inline RenderApi GetRenderApi() { return m_configuration.renderApi; }
        inline const Window &GetActiveWindow() const { return *m_window; }
        inline static Application *GetInstance() { return s_instance; }

    protected:
        Application() {}

    protected:
        Ref<Window> m_window; //has to be declated above layerstack
        AppSettings m_configuration;
        LayerStack m_layerStack;

    private:
        void HandleEvent(Event &e);

    private:
        bool m_running = false;
        static SPL_DEF Application *s_instance;    
    };

}