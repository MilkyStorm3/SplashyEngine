#include "Instrumentation.hpp"


#ifdef SPL_ENABLE_PROFILING

    #ifdef __linux__

        #define CORE_GENERAL_PROFILE_SCOPE(name) ant::InstrumentationTimer profile ## _ ## __LINE__(name, "CORE", ant::InstrumentationLevel::General)
        #define CORE_GENERAL_PROFILE_FUNC() ant::InstrumentationTimer profile ## _ ## __LINE__(__PRETTY_FUNCTION__, "CORE", ant::InstrumentationLevel::General)

        #define CORE_INTERMEDIATE_PROFILE_SCOPE(name) ant::InstrumentationTimer profile ## _ ## __LINE__(name, "CORE", ant::InstrumentationLevel::Intermediate)
        #define CORE_INTERMEDIATE_PROFILE_FUNC() ant::InstrumentationTimer profile ## _ ## __LINE__(__PRETTY_FUNCTION__, "CORE", ant::InstrumentationLevel::Intermediate)

        #define CORE_DETAILED_PROFILE_SCOPE(name) ant::InstrumentationTimer profile ## _ ## __LINE__(name, "CORE", ant::InstrumentationLevel::Detailed)
        #define CORE_DETAILED_PROFILE_FUNC() ant::InstrumentationTimer profile ## _ ## __LINE__(__PRETTY_FUNCTION__, "CORE", ant::InstrumentationLevel::Detailed)

    #endif

    #ifdef _WIN32 
    
        // todo macros for mvsc compiler

    #endif

#else

    #define CORE_GENERAL_PROFILE_SCOPE(name) ;
    #define CORE_INTERMEDIATE_PROFILE_SCOPE(name) ;
    #define CORE_DETAILED_PROFILE_SCOPE(name) ;

    #define CORE_GENERAL_PROFILE_FUNC() ;
    #define CORE_INTERMEDIATE_PROFILE_FUNC() ;
    #define CORE_DETAILED_PROFILE_FUNC() ;

#endif

