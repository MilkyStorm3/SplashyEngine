#pragma once
#ifdef SPL_ENABLE_LOGGING

#include <spdlog/logger.h>
#include <memory>

namespace ant
{
    class Logger
    {
    public:
        using logPtr = std::shared_ptr<spdlog::logger>;
        static void Init();

        static const logPtr& GetCoreLogger();
        static const logPtr& GetClientLogger();

    private:
        Logger() {}
        ~Logger() {}        
    };


    #define CORE_TRACE(...)   ant::Logger::GetCoreLogger()->trace(__VA_ARGS__);
    #define CORE_INFO(...)   ant::Logger::GetCoreLogger()->info(__VA_ARGS__);
    #define CORE_WARN(...)   ant::Logger::GetCoreLogger()->warn(__VA_ARGS__);
    #define CORE_ERROR(...)  ant::Logger::GetCoreLogger()->error(__VA_ARGS__);    

    #define SPL_SPL_ENABLE_CLIENT_LOGGING 1

} // namespace ant

#else // SPL_ENABLE_LOGGING not defined

    #define CORE_TRACE(...);
    #define CORE_INFO(...);
    #define CORE_WARN(...);
    #define CORE_ERROR(...);
  

namespace ant{ 

    class Logger
    {
    public:
        inline static void Init(){}

    private:
        Logger() {}
        ~Logger() {}
    };
}

#endif


#ifdef SPL_ENABLE_CLIENT_LOGGING

    #define APP_TRACE(...)   ant::Logger::GetClientLogger()->trace(__VA_ARGS__);
    #define APP_INFO(...)   ant::Logger::GetClientLogger()->info(__VA_ARGS__);
    #define APP_WARN(...)   ant::Logger::GetClientLogger()->warn(__VA_ARGS__);
    #define APP_ERROR(...)  ant::Logger::GetClientLogger()->error(__VA_ARGS__);

#else

    #define APP_TRACE(...);
    #define APP_INFO(...);
    #define APP_WARN(...);
    #define APP_ERROR(...);

#endif