
#ifndef __HH_ROCKHOPPER_LOGGER_
#define __HH_ROCKHOPPER_LOGGER_

#include <fmt/core.h>
#include <spdlog/fmt/ostr.h>

#include "RockHopper/Utility/String/StringUtility.hh"

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
        template <typename Format, typename... Args>
        static void Log(Instance instance, LogLevel level, Format const& format, Args&&... args);
        template <typename Format, typename... Args>
        static void LogDetailed(Instance instance, LogLevel level, std::string const& caller, Format const& format, Args&&... args);
    };

} // namespace RockHopper

// Define default value for ROCKHOPPER_LOG_LEVEL
#ifndef ROCKHOPPER_LOG_LEVEL
#define ROCKHOPPER_LOG_LEVEL 4
#elif ROCKHOPPER_LOG_LEVEL < 0
#error "ROCKHOPPER_LOG_LEVEL must be a positive integer!"
#endif

// Define default log macros
#define ROCKHOPPER_LOG_TRACE(...)
#define ROCKHOPPER_LOG_DEBUG(...)
#define ROCKHOPPER_LOG_INFO(...)
#define ROCKHOPPER_LOG_WARN(...)
#define ROCKHOPPER_LOG_ERROR(...)
#define ROCKHOPPER_LOG_FATAL(...)
#define ROCKHOPPER_INTERNAL_LOG_TRACE(...)
#define ROCKHOPPER_INTERNAL_LOG_DEBUG(...)
#define ROCKHOPPER_INTERNAL_LOG_INFO(...)
#define ROCKHOPPER_INTERNAL_LOG_WARN(...)
#define ROCKHOPPER_INTERNAL_LOG_ERROR(...)
#define ROCKHOPPER_INTERNAL_LOG_FATAL(...)

// Redefine log macros based on log level
#if ROCKHOPPER_LOG_LEVEL >= 5
#undef ROCKHOPPER_LOG_TRACE
#undef ROCKHOPPER_INTERNAL_LOG_TRACE
#define ROCKHOPPER_LOG_TRACE(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::TRACE, __VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_TRACE(...) RockHopper::Logger::LogDetailed(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::TRACE, __PRETTY_FUNCTION__, __VA_ARGS__)
#endif
#if ROCKHOPPER_LOG_LEVEL >= 4
#undef ROCKHOPPER_LOG_DEBUG
#undef ROCKHOPPER_INTERNAL_LOG_DEBUG
#define ROCKHOPPER_LOG_DEBUG(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::DEBUG, __VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_DEBUG(...) RockHopper::Logger::LogDetailed(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::DEBUG, __PRETTY_FUNCTION__, __VA_ARGS__)
#endif
#if ROCKHOPPER_LOG_LEVEL >= 3
#undef ROCKHOPPER_LOG_INFO
#undef ROCKHOPPER_INTERNAL_LOG_INFO
#define ROCKHOPPER_LOG_INFO(...)  RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::INFO,  __VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_INFO(...)  RockHopper::Logger::LogDetailed(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::INFO,  __PRETTY_FUNCTION__, __VA_ARGS__)
#endif
#if ROCKHOPPER_LOG_LEVEL >= 2
#undef ROCKHOPPER_LOG_WARN
#undef ROCKHOPPER_INTERNAL_LOG_WARN
#define ROCKHOPPER_LOG_WARN(...)  RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::WARN,  __VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_WARN(...)  RockHopper::Logger::LogDetailed(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::WARN,  __PRETTY_FUNCTION__, __VA_ARGS__)
#endif
#if ROCKHOPPER_LOG_LEVEL >= 1
#undef ROCKHOPPER_LOG_ERROR
#undef ROCKHOPPER_INTERNAL_LOG_ERROR
#define ROCKHOPPER_LOG_ERROR(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::ERROR, __VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_ERROR(...) RockHopper::Logger::LogDetailed(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::ERROR, __PRETTY_FUNCTION__, __VA_ARGS__)
#endif
#if ROCKHOPPER_LOG_LEVEL >= 0
#undef ROCKHOPPER_LOG_FATAL
#undef ROCKHOPPER_INTERNAL_LOG_FATAL
#define ROCKHOPPER_LOG_FATAL(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::FATAL, __VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_FATAL(...) RockHopper::Logger::LogDetailed(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::FATAL, __PRETTY_FUNCTION__, __VA_ARGS__)
#endif

/* ************************************************************************** */
// [Implementation] RockHopper::Logger
/* ************************************************************************** */

namespace RockHopper
{

    template <typename Format, typename... Args>
    void Logger::Log(Instance instance, LogLevel level, Format const& format, Args&&... args)
    {
        std::string const message = fmt::format(format,std::move(args)...);
        return Log(instance,level,message.c_str());
    }

    template <typename Format, typename... Args>
    void Logger::LogDetailed(Instance instance, LogLevel level, std::string const& caller, Format const& format, Args&&... args)
    {
        std::string message = fmt::format(format,std::move(args)...);
        if (caller != "") message = fmt::format("{}: {}", GetPrettyFunctionDebugName(caller), message);
        return Log(instance,level,message.c_str());
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_LOGGER_ */
