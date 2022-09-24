#include "Instrumentation.hpp"

#ifdef SPL_ENABLE_PROFILING
#include "Core/Core.hpp"

#include <thread>
#include <filesystem>

namespace ant
{
    namespace fs = std::filesystem;
    namespace cr = std::chrono;

    namespace Utils
    {
        static const char *GetProfilesFolder()
        {
            return "profiles";
        }

    } // namespace Utils

    Instrumentor::map Instrumentor::s_activeSessions;

    void Instrumentor::InitSession(const std::string &name, InstrumentationLevel level)
    {
        auto session = MakeScope<Session>(level);
        session->Start(name);

        s_activeSessions[name] = std::move(session);
    }

    Instrumentor::Session *Instrumentor::GetSession(const std::string &name)
    {
        CORE_ASSERT(s_activeSessions.find(name) != s_activeSessions.end() , "Instrumentation session {0} has not been initialized!", name);
        return s_activeSessions[name].get();
    }

    Instrumentor::Session::~Session()
    {
        Close();
    }

    void Instrumentor::Session::Start(const std::string &name)
    {
        fs::path path = Utils::GetProfilesFolder();
        path.append(name + ".json");

        m_outFile.open(path);
        m_outFile << "{\"otherData\": {},\"traceEvents\":[";
        m_outFile.flush();
        m_profileCount = 0;
    }

    void Instrumentor::Session::Close()
    {
        if (!m_outFile.is_open())
            return;

        m_outFile << "]}";
        m_outFile.flush();
        m_outFile.close();
    }

    void Instrumentor::Session::SaveProfile(const ProfileData &data)
    {
        if (!m_outFile.is_open())
            return;

        if (m_profileCount++ > 0)
            m_outFile << ",";

        std::string name = data.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_outFile << "{";
        m_outFile << "\"cat\":\"function\",";
        m_outFile << "\"dur\":" << (data.End - data.Start) << ',';
        m_outFile << "\"name\":\"" << name << "\",";
        m_outFile << "\"ph\":\"X\",";
        m_outFile << "\"pid\":0,";
        m_outFile << "\"tid\":" << data.ThreadID << ",";
        m_outFile << "\"ts\":" << data.Start;
        m_outFile << "}";

        m_outFile.flush();
    }

    InstrumentationTimer::InstrumentationTimer(const std::string &name, const std::string &targetName, InstrumentationLevel level)
    {
        m_profileData.Name = name;
        m_targetSession = Instrumentor::GetSession(targetName);

        if (m_targetSession->GetLevel() < level)
        {
            m_stopped = true;
            return;
        }

        Start();
    }

    InstrumentationTimer::~InstrumentationTimer()
    {
        Stop();
    }

    void InstrumentationTimer::Start()
    {
        m_time = cr::high_resolution_clock::now();
    }

    void InstrumentationTimer::Stop()
    {
        if (m_stopped)
            return;

        m_stopped = true;
        auto endPoint = cr::high_resolution_clock::now();

        auto start = cr::time_point_cast<cr::microseconds>(m_time).time_since_epoch().count();
        auto end = cr::time_point_cast<cr::microseconds>(endPoint).time_since_epoch().count();

        m_profileData.Start = start;
        m_profileData.End = end;
        m_profileData.ThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

        m_targetSession->SaveProfile(m_profileData);
    }

}
#endif