
#ifndef __HH_ROCKHOPPER_UTIL_STRING_UTILITY_
#define __HH_ROCKHOPPER_UTIL_STRING_UTILITY_

#include <string>

namespace RockHopper
{

    std::string& MakePrettyFunctionDebugName(std::string& name);

    inline std::string GetPrettyFunctionDebugName(std::string const& name)
    {
        std::string copy = name;
        return MakePrettyFunctionDebugName(copy);
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_UTIL_STRING_UTILITY_ */
