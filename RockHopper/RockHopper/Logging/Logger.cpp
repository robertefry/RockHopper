
#include "Logger.h"

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>
#include <unordered_map>
#include <mutex>

/* LoggerInstanceManager ******************************************************/
/******************************************************************************/
class LoggerInstanceManager
{
public:
    static std::shared_ptr<spdlog::logger>& getInstance(RockHopper::Logger::Instance instance);
private:
    using LoggerInstanceMap = std::unordered_map<RockHopper::Logger::Instance,std::shared_ptr<spdlog::logger>>;
    static LoggerInstanceMap m_LoggerInstances;
    static std::mutex m_Mutex;
};

LoggerInstanceManager::LoggerInstanceMap LoggerInstanceManager::m_LoggerInstances{};
std::mutex LoggerInstanceManager::m_Mutex{};

std::shared_ptr<spdlog::logger>& LoggerInstanceManager::getInstance(RockHopper::Logger::Instance instance)
{
    std::lock_guard<std::mutex> guard(m_Mutex);

    if (m_LoggerInstances.count(instance)) {
        return m_LoggerInstances[instance];
    }

    switch (instance)
    {
        case RockHopper::Logger::Instance::InternalLogger:
        {
            auto logger = spdlog::stdout_color_mt("RockHopper");
            logger->set_level(spdlog::level::trace);
            logger->set_pattern("%^[%T] %n: %v%$");
            return m_LoggerInstances[instance] = logger;
        }

        case RockHopper::Logger::Instance::ClientLogger:
        {
            auto logger = spdlog::stdout_color_mt("Application");
            logger->set_level(spdlog::level::trace);
            logger->set_pattern("%^[%T] %n: %v%$");
            return m_LoggerInstances[instance] = logger;
        }
    }

    // execution should never get here
    throw std::runtime_error("Logger instance not found or created.");
}
/******************************************************************************/

/* RockHopper::Logger *********************************************************/
/******************************************************************************/
void RockHopper::Logger::Log(Instance instance, LogLevel level, const char* msg)
{
    auto logger = LoggerInstanceManager::getInstance(instance);
    switch (level) {
        case LogLevel::TRACE: logger->trace(msg); break;
        case LogLevel::DEBUG: logger->debug(msg); break;
        case LogLevel::INFO: logger->info(msg); break;
        case LogLevel::WARN: logger->warn(msg); break;
        case LogLevel::ERROR: logger->error(msg); break;
        case LogLevel::FATAL: logger->critical(msg); break;
    }
}
/******************************************************************************/
