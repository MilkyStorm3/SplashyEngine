#ifdef SPL_INSTRUMENTATION
#pragma once
#include <string>
#include <fstream>
#include <chrono>

namespace ant
{

    struct ProfileData
    {
        std::string Name;
        int64_t Start, End;
        uint32_t ThreadID;
    };

    class Instrumentor
    {
    public:
        ~Instrumentor() {}

        void BeginSession(const std::string &name);
        void EndSession();

        void SaveProfile(const ProfileData &data);

        static Instrumentor *Get()
        {
            static Instrumentor instance;
            return &instance;
        }

    private:
        std::ofstream m_OutputStream;
        size_t m_ProfileCount = 0;
        Instrumentor() {}
        void WriteHeader();
        void WriteFooter();
    };

 
    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const std::string &name);
        ~InstrumentationTimer();

        void Start();
        void Stop();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_time;
        ProfileData m_profileData;
        bool m_stopped = false;
    };

    //! TODO build internal and external instrumentation tooling


#ifdef __linux__ 

// #define EDITOR_PROFILE_SCOPE(name) ant::InstrumentationTimer profile ## _ ## __LINE__(name)
// #define EDITOR_PROFILE_FUNC() ant::InstrumentationTimer profile ## _ ## __LINE__ (__PRETTY_FUNCTION__)


#define CORE_PROFILE_SCOPE(name) InstrumentationTimer profile ## _ ## __LINE__(name)
#define CORE_PROFILE_FUNC() ant::InstrumentationTimer profile ## _ ## __LINE__ (__PRETTY_FUNCTION__)

#endif

#ifdef _WIN32

#define CORE_PROFILE_SCOPE(name) InstrumentationTimer profile ## _ ## __LINE__(name)
#define CORE_PROFILE_FUNC() InstrumentationTimer profile ## _ ## __LINE__ (__FUNCTION__)

#endif
}

#else

#define CORE_PROFILE_SCOPE(name) ;
#define CORE_PROFILE_FUNC() ;

#endif