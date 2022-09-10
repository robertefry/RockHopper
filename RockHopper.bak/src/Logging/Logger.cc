
#include "RockHopper/Logging/Logger.hh"

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>
#include <unordered_map>
#include <mutex>

/* ************************************************************************** */
// LoggerInstanceManager
/* ************************************************************************** */

class LoggerInstanceManager
{
    using LoggerInstanceMap = std::unordered_map<RockHopper::Logger::Instance,std::shared_ptr<spdlog::logger>>;
public:
    static std::shared_ptr<spdlog::logger>& GetInstance(RockHopper::Logger::Instance instance);
private:
    inline static LoggerInstanceMap m_LoggerInstances{};
    inline static std::mutex m_Mutex{};
};

std::shared_ptr<spdlog::logger>& LoggerInstanceManager::GetInstance(RockHopper::Logger::Instance instance)
{
    std::lock_guard<std::mutex> guard(m_Mutex);

    if (m_LoggerInstances.count(instance)) {
        return m_LoggerInstances[instance];
    }

    switch (instance)
    {
        case RockHopper::Logger::Instance::InternalLogger:
        {
            auto logger = spdlog::stdout_color_mt("Internal");
            logger->set_level(spdlog::level::trace);
            logger->set_pattern("%^[%T] [%t] %L %n: %v%$");
            return m_LoggerInstances[instance] = logger;
        }

        case RockHopper::Logger::Instance::ClientLogger:
        {
            auto logger = spdlog::stdout_color_mt("Client");
            logger->set_level(spdlog::level::trace);
            logger->set_pattern("%^[%T] [%t] %L %n: %v%$");
            return m_LoggerInstances[instance] = logger;
        }
    }

    // execution should never get here
    throw std::runtime_error("Logger instance not found or created.");
}

/* ************************************************************************** */
// [Implementation] RockHopper::Logger
/* ************************************************************************** */

namespace RockHopper
{

    void Logger::Log(Instance instance, LogLevel level, const char* msg)
    {
        auto logger = LoggerInstanceManager::GetInstance(instance);
        switch (level) {
            case LogLevel::TRACE: logger->trace(msg); break;
            case LogLevel::DEBUG: logger->debug(msg); break;
            case LogLevel::INFO:  logger->info(msg); break;
            case LogLevel::WARN:  logger->warn(msg); break;
            case LogLevel::ERROR: logger->error(msg); break;
            case LogLevel::FATAL: logger->critical(msg); abort();
        }
    }

} // namespace RockHopper
