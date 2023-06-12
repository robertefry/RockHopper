
#include "RockHopper/Logging/LoggerCore.hh"

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace RockHopper
{

    LoggerCore& LoggerCore::GetInstance()
    {
        static LoggerCore logger;
        return logger;
    }

    struct LoggerCore::Impl
    {
        std::shared_ptr<spdlog::logger> logger;

        explicit Impl()
            : logger{spdlog::stdout_color_mt("Internal")}
        {
            logger->set_level(spdlog::level::trace);
            logger->set_pattern("%^[%T] [%t] %L %n: %v%$");
        }
    };

    LoggerCore::LoggerCore()
        : m_Impl{std::make_unique<Impl>()}
    {
    }

    void LoggerCore::log(Level level, std::string const& msg)
    {
        switch (level)
        {
        case TRACE:
            m_Impl->logger->trace(msg);
            break;

        case DEBUG:
            m_Impl->logger->debug(msg);
            break;

        case INFO:
            m_Impl->logger->info(msg);
            break;

        case WARN:
            m_Impl->logger->warn(msg);
            break;

        case ERROR:
            m_Impl->logger->error(msg);
            break;

        case FATAL:
            m_Impl->logger->critical(msg);
            std::terminate();
        }
    }

} // namespace RockHopper
