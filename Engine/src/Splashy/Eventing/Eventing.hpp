#pragma once
#include <Input/Event.hpp>
#include <tuple>
#include <functional>

namespace ant
{

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

} // namespace ant
