#pragma once
#include <tuple>
#include <functional>

namespace ant
{
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
        WindowUnfocused,
        Application
    };

    enum class EventCategory
    {
        None = 0,
        Keyboard,
        Mouse,
        Window,
        Application
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

    template <class... Args>
    class ApplicationEvent
        : public Event
    {
    public:
        using Callback = std::function<void(Args...)>;

        virtual ~ApplicationEvent() = default;
        SET_EVENT_TYPE(Application);

        // LOG_FUNC_OVERRIDE();
        ApplicationEvent(Args... args)
        {
            m_data = {args...};
        }

        const std::tuple<Args...> GetData() const { return m_data; }

    protected:
        std::tuple<Args...> m_data;
    };

    class EventCallback
        : public std::function<void(Event &)>
    {
    };

    class EventEmitter
    {
    private:
        EventEmitter() {}
        ~EventEmitter() {}
        friend class Application;
        static void SetMainEventCallback(const std::function<void(Event &)> &newCallback);

    public:
        static void Emit(Event &event)
        {
            s_MainEventCallback(event);
        }

        static const EventCallback &GetMainCallback();

    private:
        static std::function<void(Event &)> s_MainEventCallback;
    };

    class EventDispatcher
    {
    public:
        EventDispatcher() {}
        EventDispatcher(Event &e) : m_event(&e) {}
        ~EventDispatcher() {}

        void SetEventPtr(Event *e) { m_event = e; }

        template <class evType>
        bool DispatchEvent(const std::function<void(evType &)> &handler)
        {
            if (m_event->m_handled)
                return true;

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

} // namespace ant
