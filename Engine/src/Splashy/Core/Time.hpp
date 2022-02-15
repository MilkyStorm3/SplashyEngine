#pragma once

namespace ant
{
    class TimeStep
    {
    public:
        TimeStep(float time) : m_time(time) {}
        TimeStep() {}
        ~TimeStep() {}

        float Seconds() const { return m_time; }
        float MiliSeconds() const { return m_time * 1000; }

        static TimeStep GetFrameTime();
        static void UpdateFrameTime(TimeStep& lastFrameTime);

        friend TimeStep operator-(TimeStep l, TimeStep r);
        friend TimeStep operator+(TimeStep l, TimeStep r);
        friend bool operator==(TimeStep l, TimeStep r);

        operator float() const ;

    private:
        float m_time;
        static TimeStep s_frameTime;
    };

} // namespace ant