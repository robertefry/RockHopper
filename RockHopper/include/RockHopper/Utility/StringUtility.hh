
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
