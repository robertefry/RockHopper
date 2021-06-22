
#ifndef __HH_ROCKHOPPER_LOGGER_
#define __HH_ROCKHOPPER_LOGGER_

// #include "RockHopper/Utility/StringList.hh"

/* ************************************************************************** */
// [Definition] RockHopper::Logger
/* ************************************************************************** */

namespace RockHopper
{

    class Logger
    {
    public:
        enum class Instance {
            InternalLogger,
            ClientLogger,
        };
        enum class LogLevel {
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
        };
        static void Log(Instance instance, LogLevel level, const char* msg);
        // template <typename... Args>
        // static void Log(Instance instance, LogLevel level, const Args&... args);
    };

} // namespace RockHopper

// Client logger macros
#define ROCKHOPPER_LOG_TRACE(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::TRACE, __VA_ARGS__)
#define ROCKHOPPER_LOG_DEBUG(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::DEBUG, __VA_ARGS__)
#define ROCKHOPPER_LOG_INFO(...)  RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::INFO,  __VA_ARGS__)
#define ROCKHOPPER_LOG_WARN(...)  RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::WARN,  __VA_ARGS__)
#define ROCKHOPPER_LOG_ERROR(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::ERROR, __VA_ARGS__)
#define ROCKHOPPER_LOG_FATAL(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::FATAL, __VA_ARGS__)

// Internal logger macros
#define ROCKHOPPER_INTERNAL_LOG_TRACE(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::TRACE, __VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_DEBUG(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::DEBUG, __VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_INFO(...)  RockHopper::Logger::Log(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::INFO,  __VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_WARN(...)  RockHopper::Logger::Log(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::WARN,  __VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_ERROR(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::ERROR, __VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_FATAL(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::FATAL, __VA_ARGS__)

/* ************************************************************************** */
// [Implementation] RockHopper::Logger
/* ************************************************************************** */

// template <typename... Args>
// void RockHopper::Logger::Log(Instance instance, LogLevel level, const Args&... args)
// {
//     StringList stringlist;
//     for (const auto& arg : {args...}) {
//         stringlist << arg;
//     }
//     Log(instance, level, stringlist.to_string().c_str());
// }

#endif /* __HH_ROCKHOPPER_LOGGER_ */
