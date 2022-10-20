#include "Instrumentation.hpp"


#ifdef SPL_ENABLE_PROFILING

    #if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
        #define __PRETTY_FUNCTION__ __FUNCSIG__
    #endif

    #define CORE_GENERAL_PROFILE_SCOPE(name) ant::InstrumentationTimer profile ## _ ## __LINE__(name, "CORE", ant::InstrumentationLevel::General)
    #define CORE_GENERAL_PROFILE_FUNC() ant::InstrumentationTimer profile ## _ ## __LINE__(__PRETTY_FUNCTION__, "CORE", ant::InstrumentationLevel::General)

    #define CORE_INTERMEDIATE_PROFILE_SCOPE(name) ant::InstrumentationTimer profile ## _ ## __LINE__(name, "CORE", ant::InstrumentationLevel::Intermediate)
    #define CORE_INTERMEDIATE_PROFILE_FUNC() ant::InstrumentationTimer profile ## _ ## __LINE__(__PRETTY_FUNCTION__, "CORE", ant::InstrumentationLevel::Intermediate)

    #define CORE_DETAILED_PROFILE_SCOPE(name) ant::InstrumentationTimer profile ## _ ## __LINE__(name, "CORE", ant::InstrumentationLevel::Detailed)
    #define CORE_DETAILED_PROFILE_FUNC() ant::InstrumentationTimer profile ## _ ## __LINE__(__PRETTY_FUNCTION__, "CORE", ant::InstrumentationLevel::Detailed)

#else

    #define CORE_GENERAL_PROFILE_SCOPE(name) ;
    #define CORE_INTERMEDIATE_PROFILE_SCOPE(name) ;
    #define CORE_DETAILED_PROFILE_SCOPE(name) ;

    #define CORE_GENERAL_PROFILE_FUNC() ;
    #define CORE_INTERMEDIATE_PROFILE_FUNC() ;
    #define CORE_DETAILED_PROFILE_FUNC() ;

#endif

