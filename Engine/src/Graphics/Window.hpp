#pragma once
#include <stdint.h>
#include <array>
#include <string>
#include <functional>
#include <glm/glm.hpp>

class GLFWwindow;

namespace ant
{
    class Event;
    class Window
    {
    public:
        using EventCallback = std::function<void(Event &)>;

        struct Properties
        {
            uint32_t width, height;
            std::string title;
            bool vsync = false;
            bool resizable = true;
        };

    public:
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
        void SetCallbacks();
        GLFWwindow *m_nativeWindow;
        Properties m_properties;
        EventCallback m_eventCallback;
    };

} // namespace ant
