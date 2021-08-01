
#ifndef __HH_ROCKHOPPER_DEBUG_
#define __HH_ROCKHOPPER_DEBUG_

#include "RockHopper/Logging/Logger.hh"

#include <string>

class DebugName
{
public:
    virtual ~DebugName() = default;
    explicit DebugName(std::string const& type, std::string const& name)
        : m_Type{type}, m_Name{name}
    {}
    inline void set_type(std::string const& type)
    {
        m_Type = type;
    }
    inline void set_name(std::string const& name)
    {
        m_Name = name;
    }
    inline std::string str() const
    {
        return "<" + m_Type + ":" + m_Name + ">";
    }
private:
    std::string m_Type;
    std::string m_Name;
};
inline std::ostream& operator<<(std::ostream& ostr, DebugName const& debug_name)
{
    return ostr << debug_name.str();
}

#define ROCKHOPPER_ASSERT_TRACE(assert,...) if (not assert) ROCKHOPPER_LOG_TRACE(__VA_ARGS__);
#define ROCKHOPPER_ASSERT_DEBUG(assert,...) if (not assert) ROCKHOPPER_LOG_DEBUG(__VA_ARGS__);
#define ROCKHOPPER_ASSERT_INFO(assert,...)  if (not assert) ROCKHOPPER_LOG_INFO(__VA_ARGS__);
#define ROCKHOPPER_ASSERT_WARN(assert,...)  if (not assert) ROCKHOPPER_LOG_WARN(__VA_ARGS__);
#define ROCKHOPPER_ASSERT_ERROR(assert,...) if (not assert) ROCKHOPPER_LOG_ERROR(__VA_ARGS__);
#define ROCKHOPPER_ASSERT_FATAL(assert,...) if (not assert) ROCKHOPPER_LOG_FATAL(__VA_ARGS__);

#define ROCKHOPPER_INTERNAL_ASSERT_TRACE(assert,...) if (not assert) ROCKHOPPER_INTERNAL_LOG_TRACE(__VA_ARGS__);
#define ROCKHOPPER_INTERNAL_ASSERT_DEBUG(assert,...) if (not assert) ROCKHOPPER_INTERNAL_LOG_DEBUG(__VA_ARGS__);
#define ROCKHOPPER_INTERNAL_ASSERT_INFO(assert,...)  if (not assert) ROCKHOPPER_INTERNAL_LOG_INFO(__VA_ARGS__);
#define ROCKHOPPER_INTERNAL_ASSERT_WARN(assert,...)  if (not assert) ROCKHOPPER_INTERNAL_LOG_WARN(__VA_ARGS__);
#define ROCKHOPPER_INTERNAL_ASSERT_ERROR(assert,...) if (not assert) ROCKHOPPER_INTERNAL_LOG_ERROR(__VA_ARGS__);
#define ROCKHOPPER_INTERNAL_ASSERT_FATAL(assert,...) if (not assert) ROCKHOPPER_INTERNAL_LOG_FATAL(__VA_ARGS__);

#endif /* __HH_ROCKHOPPER_DEBUG_ */
