
#include "RockHopper/Logging/Logger.hh"

#include <vector>
#include <string>
#include <regex>

namespace RockHopper
{

    auto Logger::DemangleFunctionName(std::string name) -> std::string
    {
        auto const erase_delimited = [&](char char_begin, char char_end)
        {
            auto itr_a = name.begin();

            while (true)
            {
                itr_a = std::find(itr_a,name.end(),char_begin);
                if (itr_a == name.end()) return;        // break while(true)

                auto itr_b = std::next(itr_a);
                for (size_t count = 1; count > 0; ++itr_b)
                {
                    if (*itr_b == char_begin) count += 1;
                    if (*itr_b == char_end) count -= 1;
                }
                name.erase(itr_a,itr_b);
            }
        };
        auto const erase_keywords = [&](std::vector<std::string> const& keywords)
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
        auto const make_last_word = [&]()
        {
            auto itr_b = std::find_if(name.rbegin(),name.rend(),[](char chr){ return chr != ' '; });
            auto itr_a = std::find_if(itr_b,name.rend(),[](char chr){ return chr == ' '; });

            size_t const index = (size_t)(itr_a.base() - name.begin());
            size_t const length = (size_t)(itr_b.base() - itr_a.base());

            name = name.substr(index,length);
        };
        auto const erase_trim = [&]()
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
