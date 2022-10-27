#include "Pch.h"
#include <sstream>
#include "InputEvents.hpp"

namespace ant
{
    EventCategory Event::GetEventCategory()
    {
        uint32_t cat = uint32_t(m_type);

        if (cat && cat <= uint32_t(EventType::KeyReleased))
            return EventCategory::Keyboard;

        if (cat >= uint32_t(EventType::MouseButtonPressed) && cat <= uint32_t(EventType::MouseMoved))
            return EventCategory::Mouse;

        if (cat >= uint32_t(EventType::WindowRezised))
            return EventCategory::Window;

        return EventCategory::None;
    }

    std::string KeyEvent::GetStringLog()
    {
        std::stringstream o;
        o << GetLog() << " " << int(m_key) << " modifier - " << int16_t(m_modifier);
        return o.str();
    }

    std::string KeyPressedEvent::GetStringLog()
    {
        std::stringstream o;
        o << GetLog() << " " << int(m_key) << " modifier - " << int16_t(m_modifier) << " repeatig - " << (m_repeating ? "true" : "false");
        return o.str();
    }

    std::string MouseButtonEvent::GetStringLog()
    {
        std::stringstream o;
        o << GetLog() << " " << int(m_button);
        return o.str();
    }

    std::string MouseScrolledEvent::GetStringLog()
    {
        std::stringstream o;
        o << GetLog() << " x - " << m_scrollData.x << " y - " << m_scrollData.y;
        return o.str();
    }

    std::string MouseMovedEvent::GetStringLog()
    {
        std::stringstream o;
        o << GetLog() << " x - " << m_position.x << " y - " << m_position.y;
        return o.str();
    }

    std::string WindowRezisedEvent::GetStringLog()
    {
        std::stringstream o;
        o << GetLog() << " size - " << m_size.x << " x " << m_size.y;
        return o.str();
    }

    bool KeyEvent::HasModifier(KeyModifier mod) const
    {
        return int16_t(mod) & int16_t(m_modifier);
    }

    bool MouseButtonEvent::HasModifier(KeyModifier mod) const
    {
        return int16_t(mod) & int16_t(m_modifier);
    }

} // namespace ant