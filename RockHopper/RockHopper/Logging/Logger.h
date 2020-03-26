
#pragma once

#include "RockHopper/Utility/StringBuilder.h"

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
        template <typename... Args>
        static void Log(Instance instance, LogLevel level, const Args&... args);
    };
}

// Client logger macros
#define ROCKHOPPER_LOG_TRACE(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::TRACE, __VA_ARGS__)
#define ROCKHOPPER_LOG_DEBUG(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::DEBUG, __VA_ARGS__)
#define ROCKHOPPER_LOG_INFO(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::INFO, __VA_ARGS__)
#define ROCKHOPPER_LOG_WARN(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::WARN, __VA_ARGS__)
#define ROCKHOPPER_LOG_ERROR(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::ERROR, __VA_ARGS__)
#define ROCKHOPPER_LOG_FATAL(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::ClientLogger, RockHopper::Logger::LogLevel::FATAL, __VA_ARGS__)

// Internal logger macros
#define ROCKHOPPER_LOGINTERNAL_INFO(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::INFO, __VA_ARGS__)
#define ROCKHOPPER_LOGINTERNAL_WARN(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::WARN, __VA_ARGS__)
#define ROCKHOPPER_LOGINTERNAL_ERROR(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::ERROR, __VA_ARGS__)
#define ROCKHOPPER_LOGINTERNAL_FATAL(...) RockHopper::Logger::Log(RockHopper::Logger::Instance::InternalLogger, RockHopper::Logger::LogLevel::FATAL, __VA_ARGS__)

/******************************************************************************/
/* [Implementation] RockHopper::Logger ****************************************/
/******************************************************************************/

template <typename... Args>
void RockHopper::Logger::Log(Instance instance, LogLevel level, const Args&... args)
{
    StringBuilder builder;
    for (const auto& arg : {args...}) {
        builder << arg;
    }
    Log(instance, level, builder.str().c_str());
}
