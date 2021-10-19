#include "Pch.h"
#include "debug/Instrumentation.hpp"
#include <thread>
#include <algorithm>
#include "Core/Core.hpp"

namespace ant
{
    void Instrumentor::BeginSession(const std::string &name)
    {
        m_OutputStream.open(name + "_profiles.json");
        WriteHeader();
    }

    void Instrumentor::EndSession()
    {
        m_ProfileCount = 0;
        WriteFooter();
        m_OutputStream.close();
    }

    void Instrumentor::SaveProfile(const ProfileData &data)
    {
        if (m_ProfileCount++ > 0)
            m_OutputStream << ",";

        std::string name = data.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"function\",";
        m_OutputStream << "\"dur\":" << (data.End - data.Start) << ',';
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << data.ThreadID << ",";
        m_OutputStream << "\"ts\":" << data.Start;
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    void Instrumentor::WriteHeader()
    {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";        
        m_OutputStream.flush();
        m_ProfileCount = 0;
    }

    void Instrumentor::WriteFooter()
    {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

    InstrumentationTimer::InstrumentationTimer(const std::string &name)
    {
        m_profileData.Name = name;
        Start();
    }

    InstrumentationTimer::~InstrumentationTimer()
    {
        if (!m_stopped)
            Stop();
    }

    void InstrumentationTimer::Start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }

    void InstrumentationTimer::Stop()
    {
        m_stopped = true;
        auto endPoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_time).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();

        m_profileData.Start = start;
        m_profileData.End = end;
        m_profileData.ThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

        Instrumentor::Get()->SaveProfile(m_profileData);
    }

}