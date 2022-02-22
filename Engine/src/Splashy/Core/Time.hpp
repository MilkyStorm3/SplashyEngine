#pragma once
#include <chrono>

namespace ant
{

    class TimeStep
    {
    public:
        TimeStep(int64_t step) : m_stepAsMicroSeconds(step) {}
        ~TimeStep() {}

        double Seconds() const;
        double MilliSeconds() const;
        int64_t MicroSeconds() const;

        friend bool operator==(TimeStep l, TimeStep r);

    private:
        int64_t m_stepAsMicroSeconds;
    };

    class TimePoint
    {
        using Point = std::chrono::time_point<std::chrono::high_resolution_clock>;

    public:
        TimePoint(const Point &time) : m_time(time) {}
        ~TimePoint() {}

        int64_t Seconds() const;
        int64_t MilliSeconds() const;
        int64_t MicroSeconds() const;

        friend TimeStep operator-(TimePoint l, TimePoint r);
        friend bool operator==(TimePoint l, TimePoint r);

    private:
        Point m_time;
    };

    class Time
    {
    public:
        static TimePoint Now();

    private:
        Time() {}
        ~Time() {}
    };

} // namespace ant