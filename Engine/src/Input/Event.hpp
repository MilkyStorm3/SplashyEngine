#pragma once
#include <stdint.h>
#include <string>
#include <functional>
#include <vector>
#include <typeinfo>
#include <utility>
#include "Input/KeyCodes.hpp"

namespace ant
{
    using KeycodeType = KeyCode;
    using ButtoncodeType = MouseButtonCode;
    using ScrollType = double;
    using MouseOffsetType = double;
    using WindowSizeType = int32_t;

    enum class EventType
    {
        None = 0,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseScrolled,
        MouseMoved,
        WindowRezised,
        WindowMinimalized, // missing implemenration
        WindowClosed,
        WindowFocused,
        WindowUnfocused
    };

    enum class EventCategory
    {
        None = 0,
        Keyboard,
        Mouse,
        Window
    };

#define SET_EVENT_TYPE(event_type)                                          \
    virtual const char *GetLog() override { return "Event: " #event_type; } \
    inline static EventType GetStaticType() { return EventType::event_type; }

#define LOG_FUNC_OVERRIDE() virtual std::string GetStringLog() override;

    class Event
    {
    public:
        virtual ~Event() {}

        virtual std::string GetStringLog() { return GetLog(); } // override manually
        inline const EventType &GetEventType() { return m_type; }
        virtual const char *GetLog() = 0;
        void MarkHandled(bool state = true) { m_handled = state; }
        friend class EventDispatcher;
        friend class LayerStack;

        EventCategory GetEventCategory();
        bool IsInCategory(EventCategory category) { return GetEventCategory() == category; }

    protected:
        Event(EventType type) : m_type(type) {}

        EventType m_type;
        bool m_handled = false;
    };

    class EventDispatcher
    {
    public:
        EventDispatcher() {}
        EventDispatcher(Event &e) : m_event(&e) {}
        ~EventDispatcher() {}

        void SetEventPtr(Event *e) { m_event = e; }

        template <class evType>
        bool DispatchEvent(const std::function<void(evType &)> &handler) //?check these casts
        {

            if (evType::GetStaticType() == m_event->GetEventType())
            {
                handler(static_cast<evType &>(*m_event));
                return true;
            }
            return false;
        }

    private:
        Event *m_event;
    };

    // KeyEvents

    class KeyEvent
        : public Event
    {
    public:
        virtual ~KeyEvent() {}

        LOG_FUNC_OVERRIDE();
        inline KeycodeType GetKeyCode() { return m_key; }
        bool HasModifier(KeyModifier mod) const;

    protected:
        KeyEvent(KeycodeType key, KeyModifier modifier, EventType type) : Event(type), m_key(key), m_modifier(modifier) {}
        KeycodeType m_key;
        KeyModifier m_modifier;
    };

    class KeyPressedEvent
        : public KeyEvent
    {
    public:
        KeyPressedEvent(KeycodeType key, KeyModifier modifier, bool repeating = false) : KeyEvent(key, modifier, EventType::KeyPressed), m_repeating(repeating) {}
        ~KeyPressedEvent() {}
        SET_EVENT_TYPE(KeyPressed);
        LOG_FUNC_OVERRIDE();
        inline bool IsRepeating() { return m_repeating; }

    private:
        bool m_repeating;
    };

    class KeyReleasedEvent
        : public KeyEvent
    {
    public:
        KeyReleasedEvent(KeycodeType key, KeyModifier modifier) : KeyEvent(key, modifier, EventType::KeyReleased) {}
        ~KeyReleasedEvent() {}

        SET_EVENT_TYPE(KeyReleased);
    };

    //MouseEvents

    class MouseButtonEvent
        : public Event
    {
    public:
        virtual ~MouseButtonEvent() {}
        inline ButtoncodeType GetButtonCode() { return m_button; }
        bool HasModifier(KeyModifier mod) const;
        LOG_FUNC_OVERRIDE();

    protected:
        MouseButtonEvent(EventType type, ButtoncodeType key, KeyModifier modifier) : Event(type), m_button(key), m_modifier(modifier) {}

        KeyModifier m_modifier;
        ButtoncodeType m_button;
    };

    class MouseButtonPressedEvent
        : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(ButtoncodeType button, KeyModifier modifier)
            : MouseButtonEvent(EventType::MouseButtonPressed, button, modifier) {}

        ~MouseButtonPressedEvent() {}
        SET_EVENT_TYPE(MouseButtonPressed);
    };

    class MouseButtonReleasedEvent
        : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(ButtoncodeType button, KeyModifier modifier) : MouseButtonEvent(EventType::MouseButtonReleased, button, modifier) {}
        ~MouseButtonReleasedEvent() {}
        SET_EVENT_TYPE(MouseButtonReleased);
    };

    class MouseScrolledEvent
        : public Event
    {
    public:
        struct ScrollData
        {
            ScrollType x, y;
        };

    public:
        MouseScrolledEvent(const ScrollData &scrollData) : Event(EventType::MouseScrolled), m_scrollData(scrollData) {}
        const ScrollData &GetScrollOffset() { return m_scrollData; }
        ~MouseScrolledEvent() {}
        SET_EVENT_TYPE(MouseScrolled);
        LOG_FUNC_OVERRIDE();

    private:
        ScrollData m_scrollData;
    };

    class MouseMovedEvent
        : public Event
    {
    public:
        struct MousePosData
        {
            MouseOffsetType x, y;
        };

    public:
        MouseMovedEvent(MousePosData pos) : Event(EventType::MouseMoved), m_position(pos) {}
        ~MouseMovedEvent() {}
        SET_EVENT_TYPE(MouseMoved);
        LOG_FUNC_OVERRIDE();
        inline const MousePosData &GetMousePos() { return m_position; }

    private:
        MousePosData m_position;
    };

    //WindowEvents

    class WindowRezisedEvent
        : public Event
    {
    public:
        struct WindowSize
        {
            WindowSizeType x, y;
        };

    public:
        WindowRezisedEvent(WindowSize size) : Event(EventType::WindowRezised), m_size(size) {}
        ~WindowRezisedEvent() {}
        SET_EVENT_TYPE(WindowRezised);
        inline const WindowSize &GetWindowSize() { return m_size; }
        LOG_FUNC_OVERRIDE();

    private:
        WindowSize m_size;
    };

    class WindowMinimalizedEvent
        : public Event
    {
    public:
        WindowMinimalizedEvent() : Event(EventType::WindowMinimalized) {}
        ~WindowMinimalizedEvent() {}
        SET_EVENT_TYPE(WindowRezised);
    };

    class WindowClosedEvent
        : public Event
    {
    public:
        WindowClosedEvent() : Event(EventType::WindowClosed) {}
        ~WindowClosedEvent() {}
        SET_EVENT_TYPE(WindowClosed);
    };

    class WindowFocusedEvent
        : public Event
    {
    public:
        WindowFocusedEvent() : Event(EventType::WindowFocused) {}
        ~WindowFocusedEvent() {}
        SET_EVENT_TYPE(WindowFocused);
    };

    class WindowUnfocusedEvent
        : public Event
    {
    public:
        WindowUnfocusedEvent() : Event(EventType::WindowUnfocused) {}
        ~WindowUnfocusedEvent() {}
        SET_EVENT_TYPE(WindowUnfocused);
    };

} // namespace ant
