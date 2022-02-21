#include "Instrumentation.hpp"


#ifdef SPL_INSTRUMENTATION

    #ifdef __linux__

        #define CORE_GENERAL_PROFILE_SCOPE(name) ant::InstrumentationTimer profile ## _ ## __LINE__(name, "CORE", ant::InstrumentationLevel::General)
        #define CORE_GENERAL_PROFILE_FUNC() ant::InstrumentationTimer profile ## _ ## __LINE__(__PRETTY_FUNCTION__, "CORE", ant::InstrumentationLevel::General)

        #define CORE_INTERMEDIATE_PROFILE_SCOPE(name) ant::InstrumentationTimer profile ## _ ## __LINE__(name, "CORE", ant::InstrumentationLevel::Intermediate)
        #define CORE_INTERMEDIATE_PROFILE_FUNC() ant::InstrumentationTimer profile ## _ ## __LINE__(__PRETTY_FUNCTION__, "CORE", ant::InstrumentationLevel::Intermediate)

        #define CORE_DETAILED_PROFILE_SCOPE(name) ant::InstrumentationTimer profile ## _ ## __LINE__(name, "CORE", ant::InstrumentationLevel::Detailed)
        #define CORE_DETAILED_PROFILE_FUNC() ant::InstrumentationTimer profile ## _ ## __LINE__(__PRETTY_FUNCTION__, "CORE", ant::InstrumentationLevel::Detailed)

    #endif

    #ifdef _WIN32 // todo adjust for mvsc compiler

    #define CORE_PROFILE_SCOPE(name) InstrumentationTimer profile##_##__LINE__(name)
    #define CORE_PROFILE_FUNC() InstrumentationTimer profile##_##__LINE__(__FUNCTION__)

    #endif

#endif

