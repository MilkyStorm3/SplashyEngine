#pragma once
#include <stdint.h>
#include <string>
#include <functional>
#include <glm/vec2.hpp>
#include "Core/Core.hpp"

namespace ant
{

    class Event;
    class Window
    {
    protected:
        using EventCallback = std::function<void(Event &)>;

    public:
        static Ref<Window> Create();

        Window() {}
        virtual ~Window() {}

        virtual void Init(const std::string &title, uint32_t width, uint32_t height, bool resizeable, bool vsync) = 0;
        virtual void Update() = 0;

        virtual void SetEventCallback(const EventCallback &callback) = 0;
        virtual void SetVsync(bool vsync) = 0;
        virtual bool IsVsync() const = 0;
        virtual void *GetNativeWindow() const = 0;

        virtual glm::ivec2 GetSize() const = 0;
    };

} // namespace ant
