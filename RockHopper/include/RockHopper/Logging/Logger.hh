
#ifndef ROCKHOPPER_LOGGING_LOGGER_HH
#define ROCKHOPPER_LOGGING_LOGGER_HH

#include <fmt/format.h>
#include <string>

namespace RockHopper
{

    class Logger
    {
    public:
        enum Level
        {
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
        };

        virtual void log(Level level, std::string const& msg) = 0;

    public:

        static std::string DemangleFunctionName(std::string name);

        template <bool use_caller, typename... Args>
        [[nodiscard]] static auto PreProcessMessage(std::string const& caller,
            fmt::format_string<Args...> const& fmt, Args&&... args)
        {
            auto message = fmt::format(fmt,std::forward<Args>(args)...);

            if constexpr (use_caller) {
                message = fmt::format("{}: {}",DemangleFunctionName(caller),message);
            }
            return message;
        }
    };

} // namespace RockHopper

#endif /* ROCKHOPPER_LOGGING_LOGGER_HH */

// Define default value for ROCKHOPPER_LOG_LEVEL
#ifndef ROCKHOPPER_LOG_LEVEL
#define ROCKHOPPER_LOG_LEVEL 4
#elif ROCKHOPPER_LOG_LEVEL < 0
#error "ROCKHOPPER_LOG_LEVEL must be a positive integer!"
#endif

// Define default log macros
#include "Logger.disable.hh"
