#pragma once
#include <stdint.h>
#include <array>
#include <string>
#include <functional>
#include <glm/glm.hpp>

class GLFWwindow;

namespace df
{
    class Event;
    class Window
    {
    public: // public data types
        using EventCallback = std::function<void(Event *)>;

        struct Properties
        {
            uint32_t width, height;
            std::string title;
            bool vsync = false;
            bool resizable = true;
        };

    private: // private data types
        class GlLibsWrangler
        {
        public:
            void InitGlfw();
            void InitGlad();
            void DeactivateGlfw();

        private:
            static bool s_glProfileSet;
            static bool s_gladLoaded;
        };

    public: //public functions
        Window() {}
        ~Window();
        void Init(const Properties &props);
        void Update();
        void SetEventCallback(EventCallback callback) { m_eventCallback = callback; }
        EventCallback GetEventCallback() { return m_eventCallback; }
        inline GLFWwindow *GetNativeWindow() { return m_nativeWindow; }
        inline glm::ivec2 GetSize() { return {m_properties.width, m_properties.height}; }

    private: // private functions for updating window size
        friend std::function<void(GLFWwindow *window, int width, int height)> f();
        friend class Input;
        void SetWindowSize(int width, int height);

    private: //member varibles
        GLFWwindow *m_nativeWindow;
        Properties m_properties;
        GlLibsWrangler m_glLibsWrangler;
        EventCallback m_eventCallback;
    };

} // namespace df
