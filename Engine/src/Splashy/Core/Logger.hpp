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

        static const logPtr &GetCoreLogger();
        static const logPtr &GetClientLogger();
        static const logPtr GetLogger(const char *name);
        static logPtr RegisterLogger(const char *name);

    private:
        Logger() {}
        ~Logger() {}
    };

#define CORE_TRACE(...) ant::Logger::GetCoreLogger()->trace(__VA_ARGS__);
#define CORE_INFO(...) ant::Logger::GetCoreLogger()->info(__VA_ARGS__);
#define CORE_WARN(...) ant::Logger::GetCoreLogger()->warn(__VA_ARGS__);
#define CORE_ERROR(...) ant::Logger::GetCoreLogger()->error(__VA_ARGS__);

#define APP_TRACE(...) ant::Logger::GetClientLogger()->trace(__VA_ARGS__);
#define APP_INFO(...) ant::Logger::GetClientLogger()->info(__VA_ARGS__);
#define APP_WARN(...) ant::Logger::GetClientLogger()->warn(__VA_ARGS__);
#define APP_ERROR(...) ant::Logger::GetClientLogger()->error(__VA_ARGS__);

#define TARGET_TRACE(target, ...) ant::Logger::GetLogger(#target)->trace(__VA_ARGS__);
#define TARGET_INFO(target, ...) ant::Logger::GetLogger(#target)->info(__VA_ARGS__);
#define TARGET_WARN(target, ...) ant::Logger::GetLogger(#target)->warn(__VA_ARGS__);
#define TARGET_ERROR(target, ...) ant::Logger::GetLogger(#target)->error(__VA_ARGS__);

} // namespace ant

#else // SPL_ENABLE_LOGGING not defined

#define CORE_TRACE(...) ;
#define CORE_INFO(...) ;
#define CORE_WARN(...) ;
#define CORE_ERROR(...) ;

#define APP_TRACE(...) ;
#define APP_INFO(...) ;
#define APP_WARN(...) ;
#define APP_ERROR(...) ;

#define TARGET_TRACE(target, ...) ;
#define TARGET_INFO(target, ...) ;
#define TARGET_WARN(target, ...) ;
#define TARGET_ERROR(target, ...) ;

#endif