#include "Core/Time.hpp"
#include <GLFW/glfw3.h>

namespace ant
{
    namespace cr = std::chrono;

    double TimeStep::Seconds() const
    {
        return double(m_stepAsMicroSeconds) / 1000000.f;
    }

    double TimeStep::MilliSeconds() const
    {
        return double(m_stepAsMicroSeconds) / 1000.f;
    }

    int64_t TimeStep::MicroSeconds() const
    {
        return m_stepAsMicroSeconds;
    }

    TimePoint Time::Now()
    {
        return cr::high_resolution_clock::now();
    }

    bool operator==(TimeStep l, TimeStep r)
    {
        return l.m_stepAsMicroSeconds == r.m_stepAsMicroSeconds;
    }

    int64_t TimePoint::Seconds() const
    {
        return cr::time_point_cast<cr::seconds>(m_time).time_since_epoch().count();
    }

    int64_t TimePoint::MilliSeconds() const
    {
        return cr::time_point_cast<cr::milliseconds>(m_time).time_since_epoch().count();
    }

    int64_t TimePoint::MicroSeconds() const
    {
        return cr::time_point_cast<cr::microseconds>(m_time).time_since_epoch().count();
    }

    bool operator==(TimePoint l, TimePoint r)
    {
        return (l.m_time == r.m_time);
    }

    TimeStep operator-(TimePoint l, TimePoint r)
    {
        return cr::duration_cast<cr::microseconds>(l.m_time - r.m_time).count();
    }

} // namespace ant