
#include "RockHopper/Utility/String/StringUtility.hh"

#include <regex>

namespace RockHopper
{

    std::string& MakePrettyFunctionDebugName(std::string& name)
    {
        std::function const erase_delimited = [&](char a, char b)
        {
            auto itr_a = name.begin();

            while (true)
            {
                itr_a = std::find(itr_a,name.end(),a);
                if (itr_a == name.end()) return;        // break while(true)

                auto itr_b = std::next(itr_a);
                for (size_t count = 1; count > 0; ++itr_b)
                {
                    if (*itr_b == a) count += 1;
                    if (*itr_b == b) count -= 1;
                }
                name.erase(itr_a,itr_b);
            }
        };
        std::function const erase_keywords = [&](std::vector<std::string> const& keywords)
        {
            for (auto const& keyword : keywords)
            {
                auto const regex = std::regex("(^|\\s)"+keyword+"($|\\s)");

                while (std::regex_search(name,regex))
                {
                    name = std::regex_replace(name,regex," ");
                }
            }
        };
        std::function const make_last_word = [&]()
        {
            auto itr_b = std::find_if(name.rbegin(),name.rend(),[](char c){ return c != ' '; });
            auto itr_a = std::find_if(itr_b,name.rend(),[](char c){ return c == ' '; });

            size_t index = itr_a.base() - name.begin();
            size_t length = itr_b.base() - itr_a.base();

            name = name.substr(index,length);
        };
        std::function const erase_trim = [&]()
        {
            name = std::regex_replace(name,std::regex("^[^a-zA-Z0-9_]+"),"");
            name = std::regex_replace(name,std::regex("[^a-zA-Z0-9_]+$"),"");
        };

        erase_delimited('(',')');
        erase_delimited('[',']');
        erase_delimited('<','>');
        erase_keywords({"const(expr|eval)?"});
        make_last_word();
        erase_trim();

        return name;
    }

} // namespace RockHopper
