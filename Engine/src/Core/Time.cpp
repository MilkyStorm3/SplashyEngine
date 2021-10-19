#include "Core/Time.hpp"
#include <GLFW/glfw3.h>

namespace df
{
    TimeStep TimeStep::s_frameTime;

    TimeStep TimeStep::GetFrameTime()
    {
        return s_frameTime;
    }

    void TimeStep::UpdateFrameTime(TimeStep& lastFrameTime)
    {
        TimeStep time = glfwGetTime();
        s_frameTime = time - lastFrameTime;
        lastFrameTime = time;
    }
    
    TimeStep::operator float() const 
    {
        return m_time;
    }

    bool operator==(TimeStep l, TimeStep r)
    {
        return (l.m_time == r.m_time);
    }

    TimeStep operator+(TimeStep l, TimeStep r)
    {
        return (l.m_time + r.m_time);
    }

    TimeStep operator-(TimeStep l, TimeStep r)
    {
        return (l.m_time - r.m_time);
    }

} // namespace df