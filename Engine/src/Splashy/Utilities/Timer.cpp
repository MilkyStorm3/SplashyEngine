#include "Timer.hpp"

namespace ant
{
    namespace cr = std::chrono;

    Timer::Timer(bool autostart)
    {
        if (autostart)
            Start();
    }

    void Timer::Start()
    {
        m_timeStarted = cr::high_resolution_clock::now();
    }

    int64_t Timer::ElapsedSeconds()
    {
        return cr::duration_cast<cr::seconds>(cr::high_resolution_clock::now() - m_timeStarted).count();
    }

    int64_t Timer::ElapsedMillis()
    {
        return cr::duration_cast<cr::milliseconds>(cr::high_resolution_clock::now() - m_timeStarted).count();
    }

    int64_t Timer::ElapsedMicros()
    {
        return cr::duration_cast<cr::microseconds>(cr::high_resolution_clock::now() - m_timeStarted).count();
    }
}