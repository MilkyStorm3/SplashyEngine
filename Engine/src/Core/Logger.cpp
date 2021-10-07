#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace ant
{
    Logger::logPtr Logger::s_coreLogger;
    Logger::logPtr Logger::s_clientLogger;

    void Logger::Init()
    {
        s_coreLogger = spdlog::stdout_color_mt("Core");
        s_clientLogger = spdlog::stdout_color_mt("Client");

        s_coreLogger->set_pattern("%H:%M:%S [%n: %^%l%$]-> %v");
        s_clientLogger->set_pattern("%H:%M:%S [%n: %^%l%$]-> %v");

        s_coreLogger->set_level(spdlog::level::trace);
        s_clientLogger->set_level(spdlog::level::trace);

        s_coreLogger->flush_on(spdlog::level::trace);
        s_clientLogger->flush_on(spdlog::level::trace);
    }

    const Logger::logPtr &Logger::GetCoreLogger()
    {
        return s_coreLogger;
    }

    const Logger::logPtr &Logger::GetClientLogger() 
    {
        return s_clientLogger;
    }

} // namespace ant