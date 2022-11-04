#include "Eventing.hpp"

namespace ant
{
    std::function<void(Event &)> EventEmitter::s_MainEventCallback;

    void EventEmitter::SetMainEventCallback(const std::function<void(Event &)> &newCallback)
    {
        s_MainEventCallback = newCallback;
    }

    const EventCallback &EventEmitter::GetMainCallback()
    {
        return *(EventCallback *)&s_MainEventCallback;
    }

}