#include "Logger.hpp"
#ifdef SPL_ENABLE_LOGGING
#include <spdlog/sinks/file_sinks.h>
#include <spdlog/spdlog.h>

namespace ant
{

    static struct LoggerData
    {
        Logger::logPtr coreLogger;
        Logger::logPtr clientLogger;

        std::shared_ptr<spdlog::sinks::simple_file_sink_mt> fileSink;

    } loggerData;

    inline static void AddFileSink(Logger::logPtr logger, std::shared_ptr<spdlog::sinks::simple_file_sink_mt> sink)
    {
        ((std::vector<spdlog::sink_ptr> *)&logger->sinks())->push_back(sink);
    }

    void Logger::Init()
    {
        static bool initialized;
        if (!initialized)
        {
            initialized = true;

            loggerData.fileSink = std::make_shared<spdlog::sinks::simple_file_sink_mt>("log.splog");

            loggerData.coreLogger = RegisterLogger("Core");
            loggerData.clientLogger = RegisterLogger("Client");
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

    Logger::logPtr Logger::RegisterLogger(const char *name)
    {
        Init();
        auto logger = spdlog::stdout_color_mt(name);
        AddFileSink(logger, loggerData.fileSink);

        logger->set_pattern("%H:%M:%S [%n: %^%l%$]-> %v");
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);

        return logger;
    }

    const Logger::logPtr Logger::GetLogger(const char *name)
    {
        return spdlog::get(name);
    }

} // namespace ant

#endif