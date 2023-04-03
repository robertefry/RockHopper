
#ifndef ROCKHOPPER_LOGGING_LOGGER_CORE_HH
#define ROCKHOPPER_LOGGING_LOGGER_CORE_HH

#include "Logger.hh"

#include <memory>

namespace RockHopper
{

    class LoggerCore : public Logger
    {
    public:
        static LoggerCore& GetInstance();

    public:
        virtual ~LoggerCore() = default;
        explicit LoggerCore();

        virtual void log(Level level, std::string const& msg) override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_Impl;
    };

} // namespace RockHopper

#endif /* ROCKHOPPER_LOGGING_LOGGER_CORE_HH */
