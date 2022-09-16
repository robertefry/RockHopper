
#ifndef __HH_ROCKHOPPER_LOGGER_
#define __HH_ROCKHOPPER_LOGGER_

#include <fmt/core.h>
#include <spdlog/fmt/ostr.h>

#include <string>

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

    public:
        template <typename Format, typename... Args>
        static void LogF(Instance instance, LogLevel level, Format const& format, Args&&... args);
        template <typename Format, typename... Args>
        static void LogFC(Instance instance, LogLevel level, std::string const& caller, Format const& format, Args&&... args);

    private:
        static std::string GetPrettyCallerName(std::string const&);
    };

} // namespace RockHopper

// Define default value for ROCKHOPPER_LOG_LEVEL
#ifndef ROCKHOPPER_LOG_LEVEL
#define ROCKHOPPER_LOG_LEVEL 4
#elif ROCKHOPPER_LOG_LEVEL < 0
#error "ROCKHOPPER_LOG_LEVEL must be a positive integer!"
#endif

// The table below shows how logging macros are defined for a given `ROCKHOPPER_LOG_LEVEL`.
//
// KEY: 2 -> define with caller, 1 -> define without caller
//
//  ROCKHOPPER_LOG_LEVEL    FTL ERR WRN INF DEB TRC
//   1 (MinSizeRel    )      2
//   2 (Release       )      2   1   1
//   3 (RelWithDebInfo)      2   1   1   1
//   4 (Debug         )      2   2   2   2   2
//   5 (DebugWithTrace)      2   2   2   2   2   2
//

// Define ROCKHOPPER_LOG_FATAL
#if 1 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 5
#define ROCKHOPPER_LOG_FATAL(...) ::RockHopper::Logger::LogFC(::RockHopper::Logger::Instance::ClientLogger,::RockHopper::Logger::LogLevel::FATAL,__PRETTY_FUNCTION__,__VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_FATAL(...) ::RockHopper::Logger::LogFC(::RockHopper::Logger::Instance::InternalLogger,::RockHopper::Logger::LogLevel::FATAL,__PRETTY_FUNCTION__,__VA_ARGS__)
#else
#define ROCKHOPPER_LOG_FATAL(...)
#define ROCKHOPPER_INTERNAL_LOG_FATAL(...)
#endif

// Define ROCKHOPPER_LOG_ERROR
#if 2 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 3
#define ROCKHOPPER_LOG_ERROR(...) ::RockHopper::Logger::LogF(::RockHopper::Logger::Instance::ClientLogger,::RockHopper::Logger::LogLevel::ERROR,__VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_ERROR(...) ::RockHopper::Logger::LogF(::RockHopper::Logger::Instance::InternalLogger,::RockHopper::Logger::LogLevel::ERROR,__VA_ARGS__)
#elif 4 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 5
#define ROCKHOPPER_LOG_ERROR(...) ::RockHopper::Logger::LogFC(::RockHopper::Logger::Instance::ClientLogger,::RockHopper::Logger::LogLevel::ERROR,__PRETTY_FUNCTION__,__VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_ERROR(...) ::RockHopper::Logger::LogFC(::RockHopper::Logger::Instance::InternalLogger,::RockHopper::Logger::LogLevel::ERROR,__PRETTY_FUNCTION__,__VA_ARGS__)
#else
#define ROCKHOPPER_LOG_ERROR(...)
#define ROCKHOPPER_INTERNAL_LOG_ERROR(...)
#endif

// Define ROCKHOPPER_LOG_WARN
#if 2 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 3
#define ROCKHOPPER_LOG_WARN(...) ::RockHopper::Logger::LogF(::RockHopper::Logger::Instance::ClientLogger,::RockHopper::Logger::LogLevel::WARN,__VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_WARN(...) ::RockHopper::Logger::LogF(::RockHopper::Logger::Instance::InternalLogger,::RockHopper::Logger::LogLevel::WARN,__VA_ARGS__)
#elif 4 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 5
#define ROCKHOPPER_LOG_WARN(...) ::RockHopper::Logger::LogFC(::RockHopper::Logger::Instance::ClientLogger,::RockHopper::Logger::LogLevel::WARN,__PRETTY_FUNCTION__,__VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_WARN(...) ::RockHopper::Logger::LogFC(::RockHopper::Logger::Instance::InternalLogger,::RockHopper::Logger::LogLevel::WARN,__PRETTY_FUNCTION__,__VA_ARGS__)
#else
#define ROCKHOPPER_LOG_WARN(...)
#define ROCKHOPPER_INTERNAL_LOG_WARN(...)
#endif

// Define ROCKHOPPER_LOG_INFO
#if 3 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 3
#define ROCKHOPPER_LOG_INFO(...) ::RockHopper::Logger::LogF(::RockHopper::Logger::Instance::ClientLogger,::RockHopper::Logger::LogLevel::INFO,__VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_INFO(...) ::RockHopper::Logger::LogF(::RockHopper::Logger::Instance::InternalLogger,::RockHopper::Logger::LogLevel::INFO,__VA_ARGS__)
#elif 4 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 5
#define ROCKHOPPER_LOG_INFO(...) ::RockHopper::Logger::LogFC(::RockHopper::Logger::Instance::ClientLogger,::RockHopper::Logger::LogLevel::INFO,__PRETTY_FUNCTION__,__VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_INFO(...) ::RockHopper::Logger::LogFC(::RockHopper::Logger::Instance::InternalLogger,::RockHopper::Logger::LogLevel::INFO,__PRETTY_FUNCTION__,__VA_ARGS__)
#else
#define ROCKHOPPER_LOG_INFO(...)
#define ROCKHOPPER_INTERNAL_LOG_INFO(...)
#endif

// Define ROCKHOPPER_LOG_DEBUG
#if 4 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 5
#define ROCKHOPPER_LOG_DEBUG(...) ::RockHopper::Logger::LogFC(::RockHopper::Logger::Instance::ClientLogger,::RockHopper::Logger::LogLevel::DEBUG,__PRETTY_FUNCTION__,__VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_DEBUG(...) ::RockHopper::Logger::LogFC(::RockHopper::Logger::Instance::InternalLogger,::RockHopper::Logger::LogLevel::DEBUG,__PRETTY_FUNCTION__,__VA_ARGS__)
#else
#define ROCKHOPPER_LOG_DEBUG(...)
#define ROCKHOPPER_INTERNAL_LOG_DEBUG(...)
#endif

// Define ROCKHOPPER_LOG_TRACE
#if 5 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 5
#define ROCKHOPPER_LOG_TRACE(...) ::RockHopper::Logger::LogFC(::RockHopper::Logger::Instance::ClientLogger,::RockHopper::Logger::LogLevel::TRACE,__PRETTY_FUNCTION__,__VA_ARGS__)
#define ROCKHOPPER_INTERNAL_LOG_TRACE(...) ::RockHopper::Logger::LogFC(::RockHopper::Logger::Instance::InternalLogger,::RockHopper::Logger::LogLevel::TRACE,__PRETTY_FUNCTION__,__VA_ARGS__)
#else
#define ROCKHOPPER_LOG_TRACE(...)
#define ROCKHOPPER_INTERNAL_LOG_TRACE(...)
#endif

/* ************************************************************************** */
// [Implementation] RockHopper::Logger
/* ************************************************************************** */

namespace RockHopper
{

    template <typename Format, typename... Args>
    void Logger::LogF(Instance instance, LogLevel level, Format const& format, Args&&... args)
    {
        std::string message = (sizeof...(Args) == 0) ? format : fmt::format(format,std::forward<Args>(args)...);
        return Log(instance,level,message.c_str());
    }

    template <typename Format, typename... Args>
    void Logger::LogFC(Instance instance, LogLevel level, std::string const& caller, Format const& format, Args&&... args)
    {
        std::string message = (sizeof...(Args) == 0) ? format : fmt::format(format,std::forward<Args>(args)...);
        if (caller != "") message = fmt::format("{}: {}",GetPrettyCallerName(caller),message);
        return Log(instance,level,message.c_str());
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_LOGGER_ */
