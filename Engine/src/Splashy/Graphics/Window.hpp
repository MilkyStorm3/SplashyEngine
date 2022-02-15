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
            bool resizeable = true;
        };

    public:
        Window(){}
        ~Window();

        void Init(const Properties &props);
        void Update();

        void SetEventCallback(EventCallback callback) { m_eventCallback = callback; }
        EventCallback GetEventCallback() const { return m_eventCallback; }

        inline GLFWwindow *GetNativeWindow() const { return m_nativeWindow; }
        inline glm::ivec2 GetSize() const { return {m_properties.width, m_properties.height}; }

        void SetResizeability(bool resizeable);
        bool IsResizeable() const { return m_properties.resizeable; }

        void SetVsync(bool vsync);
        bool IsVsync() const { return m_properties.vsync; }

    private: // private functions for updating window size
        friend std::function<void(GLFWwindow *window, int width, int height)> f();
        friend class Input;
        void SetWindowSize(int width, int height);

    private: //member varibles
        GLFWwindow *m_nativeWindow;
        Properties m_properties;
        EventCallback m_eventCallback;
    };

} // namespace ant
