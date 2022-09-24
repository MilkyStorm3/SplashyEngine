#include "Logger.hpp"
#ifdef SPL_ENABLE_LOGGING
// #include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace ant
{

    static struct LoggerData
    {
        Logger::logPtr coreLogger;
        Logger::logPtr clientLogger;

    } loggerData;

    void Logger::Init()
    {
        static bool initialized;
        if (!initialized)
        {
            loggerData.coreLogger = spdlog::stdout_color_mt("Core");
            loggerData.clientLogger = spdlog::stdout_color_mt("Client");

            loggerData.coreLogger->set_pattern("%H:%M:%S [%n: %^%l%$]-> %v");
            loggerData.clientLogger->set_pattern("%H:%M:%S [%n: %^%l%$]-> %v");

            loggerData.coreLogger->set_level(spdlog::level::trace);
            loggerData.clientLogger->set_level(spdlog::level::trace);

            loggerData.coreLogger->flush_on(spdlog::level::trace);
            loggerData.clientLogger->flush_on(spdlog::level::trace);

            initialized = true;
        }
    }

    const Logger::logPtr &Logger::GetCoreLogger()
    {
        Init();
        return loggerData.coreLogger;
    }

    const Logger::logPtr &Logger::GetClientLogger()
    {
        Init();
        return loggerData.clientLogger;
    }

} // namespace ant

#endif