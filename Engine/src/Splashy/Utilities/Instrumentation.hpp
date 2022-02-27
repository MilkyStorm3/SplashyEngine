#pragma once

#ifdef SPL_ENABLE_PROFILING

#include <string>
#include <fstream>
#include <chrono>
#include <map>
#include "Core/Core.hpp"

namespace ant
{

    struct ProfileData
    {
        std::string Name;
        int64_t Start, End;
        uint32_t ThreadID;
    };

    enum class InstrumentationLevel : uint8_t
    {
        None = 0,
        General,
        Intermediate,
        Detailed
    };

    class Instrumentor
    {
        friend class InstrumentationTimer;
        class Session
        {
        public:
            Session(InstrumentationLevel l) : m_level(l) {}
            ~Session();

            void Start(const std::string &name);
            void Close();
            void SaveProfile(const ProfileData &data);

            inline InstrumentationLevel GetLevel() const { return m_level; }

        private:
            InstrumentationLevel m_level;
            std::ofstream m_outFile;
            size_t m_profileCount = 0;
        };

        using map = std::unordered_map<std::string, Scope<Session>>;

    public:
        static void InitSession(const std::string &name, InstrumentationLevel level);
        static Session *GetSession(const std::string &name);

    private:
        static map s_activeSessions;
    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const std::string &name, const std::string &targetName, InstrumentationLevel level);
        ~InstrumentationTimer();

        void Start();
        void Stop();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_time;
        ProfileData m_profileData;
        Instrumentor::Session *m_targetSession;
        bool m_stopped = false;
    };

}
#endif