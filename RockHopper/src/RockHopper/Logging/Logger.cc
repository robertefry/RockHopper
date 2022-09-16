
#include "RockHopper/Logging/Logger.hh"

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <regex>

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
    inline static LoggerInstanceMap m_LoggerMap{};
    inline static std::mutex m_LoggerMutex{};
};

std::shared_ptr<spdlog::logger>& LoggerInstanceManager::GetInstance(RockHopper::Logger::Instance instance)
{
    std::unique_lock lock {m_LoggerMutex};

    if (m_LoggerMap.count(instance)) {
        return m_LoggerMap[instance];
    }

    switch (instance)
    {
        case RockHopper::Logger::Instance::InternalLogger:
        {
            auto logger = spdlog::stdout_color_mt("Internal");
            logger->set_level(spdlog::level::trace);
            logger->set_pattern("%^[%T] [%t] %L %n: %v%$");
            return m_LoggerMap[instance] = logger;
        }

        case RockHopper::Logger::Instance::ClientLogger:
        {
            auto logger = spdlog::stdout_color_mt("Client");
            logger->set_level(spdlog::level::trace);
            logger->set_pattern("%^[%T] [%t] %L %n: %v%$");
            return m_LoggerMap[instance] = logger;
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

    std::string Logger::GetPrettyCallerName(std::string const& str)
    {
        std::string name = str;

        std::function const erase_delimited = [&](char a, char b)
        {
            auto itr_a = name.begin();

            while (true)
            {
                itr_a = std::find(itr_a,name.end(),a);
                if (itr_a == name.end()) return;        // break while(true)

                auto itr_b = std::next(itr_a);
                for (size_t count = 1; count > 0; ++itr_b)
                {
                    if (*itr_b == a) count += 1;
                    if (*itr_b == b) count -= 1;
                }
                name.erase(itr_a,itr_b);
            }
        };
        std::function const erase_keywords = [&](std::vector<std::string> const& keywords)
        {
            for (auto const& keyword : keywords)
            {
                auto const regex = std::regex("(^|\\s)"+keyword+"($|\\s)");

                while (std::regex_search(name,regex))
                {
                    name = std::regex_replace(name,regex," ");
                }
            }
        };
        std::function const make_last_word = [&]()
        {
            auto itr_b = std::find_if(name.rbegin(),name.rend(),[](char c){ return c != ' '; });
            auto itr_a = std::find_if(itr_b,name.rend(),[](char c){ return c == ' '; });

            size_t index = itr_a.base() - name.begin();
            size_t length = itr_b.base() - itr_a.base();

            name = name.substr(index,length);
        };
        std::function const erase_trim = [&]()
        {
            name = std::regex_replace(name,std::regex("^[^a-zA-Z0-9_]+"),"");
            name = std::regex_replace(name,std::regex("[^a-zA-Z0-9_]+$"),"");
        };

        erase_delimited('(',')');
        erase_delimited('[',']');
        erase_delimited('<','>');
        erase_keywords({"const(expr|eval)?"});
        make_last_word();
        erase_trim();

        return name;
    }

} // namespace RockHopper
