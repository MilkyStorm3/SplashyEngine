#pragma once
#include <chrono>

namespace ant
{

    class Timer
    {

    public:
        Timer(bool autostart = true);
        ~Timer() {}

        void Start();
        void Reset() { Start(); }

        int64_t ElapsedSeconds();
        int64_t ElapsedMillis();
        int64_t ElapsedMicros();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_timeStarted;
    };

} // namespace ant
