
#include "LoggerCore.hh"

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

#if 1 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 5
#define ROCKHOPPER_LOG_FATAL(...) ::RockHopper::LoggerCore::GetInstance().log(::RockHopper::Logger::FATAL,::RockHopper::Logger::PreProcessMessage<true>(__PRETTY_FUNCTION__,__VA_ARGS__))
#endif

#if 2 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 3
#define ROCKHOPPER_LOG_ERROR(...) ::RockHopper::LoggerCore::GetInstance().log(::RockHopper::Logger::ERROR,::RockHopper::Logger::PreProcessMessage<false>(__PRETTY_FUNCTION__,__VA_ARGS__))
#elif 4 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 5
#define ROCKHOPPER_LOG_ERROR(...) ::RockHopper::LoggerCore::GetInstance().log(::RockHopper::Logger::ERROR,::RockHopper::Logger::PreProcessMessage<true>(__PRETTY_FUNCTION__,__VA_ARGS__))
#endif

#if 2 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 3
#define ROCKHOPPER_LOG_WARN(...) ::RockHopper::LoggerCore::GetInstance().log(::RockHopper::Logger::WARN,::RockHopper::Logger::PreProcessMessage<false>(__PRETTY_FUNCTION__,__VA_ARGS__))
#elif 4 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 5
#define ROCKHOPPER_LOG_WARN(...) ::RockHopper::LoggerCore::GetInstance().log(::RockHopper::Logger::WARN,::RockHopper::Logger::PreProcessMessage<true>(__PRETTY_FUNCTION__,__VA_ARGS__))
#endif

#if 3 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 3
#define ROCKHOPPER_LOG_INFO(...) ::RockHopper::LoggerCore::GetInstance().log(::RockHopper::Logger::INFO,::RockHopper::Logger::PreProcessMessage<false>(__PRETTY_FUNCTION__,__VA_ARGS__))
#elif 4 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 5
#define ROCKHOPPER_LOG_INFO(...) ::RockHopper::LoggerCore::GetInstance().log(::RockHopper::Logger::INFO,::RockHopper::Logger::PreProcessMessage<true>(__PRETTY_FUNCTION__,__VA_ARGS__))
#endif

#if 4 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 5
#define ROCKHOPPER_LOG_DEBUG(...) ::RockHopper::LoggerCore::GetInstance().log(::RockHopper::Logger::DEBUG,::RockHopper::Logger::PreProcessMessage<true>(__PRETTY_FUNCTION__,__VA_ARGS__))
#endif

#if 5 <= ROCKHOPPER_LOG_LEVEL && ROCKHOPPER_LOG_LEVEL <= 5
#define ROCKHOPPER_LOG_TRACE(...) ::RockHopper::LoggerCore::GetInstance().log(::RockHopper::Logger::TRACE,::RockHopper::Logger::PreProcessMessage<true>(__PRETTY_FUNCTION__,__VA_ARGS__))
#endif