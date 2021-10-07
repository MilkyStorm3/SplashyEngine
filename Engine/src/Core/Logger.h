#pragma once
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

    public:
        static logPtr s_coreLogger;
        static logPtr s_clientLogger;
    };

    #define CORE_TRACE(...)   ant::Logger::GetCoreLogger()->trace(__VA_ARGS__);
    #define CORE_INFO(...)   ant::Logger::GetCoreLogger()->info(__VA_ARGS__);
    #define CORE_WARN(...)   ant::Logger::GetCoreLogger()->warn(__VA_ARGS__);
    #define CORE_ERROR(...)  ant::Logger::GetCoreLogger()->error(__VA_ARGS__);
   
    #define APP_TRACE(...)   ant::Logger::GetClientLogger()->trace(__VA_ARGS__);
    #define APP_INFO(...)   ant::Logger::GetClientLogger()->info(__VA_ARGS__);
    #define APP_WARN(...)   ant::Logger::GetClientLogger()->warn(__VA_ARGS__);
    #define APP_ERROR(...)  ant::Logger::GetClientLogger()->error(__VA_ARGS__);

} // namespace ant
