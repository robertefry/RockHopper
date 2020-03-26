
#pragma once

#include <list>
#include <string>

namespace RockHopper
{

    class StringBuilder
    {
    public:
        StringBuilder();
        ~StringBuilder();

        StringBuilder(const StringBuilder&);
        StringBuilder(StringBuilder&&);
        StringBuilder& operator=(const StringBuilder&);
        StringBuilder& operator=(StringBuilder&&);

        template <typename _Tp>
        StringBuilder& operator<<(const _Tp&);

        size_t buckets() const;
        size_t size() const;
        std::string str() const;
        void clear();

    private:
        std::list<std::string> m_Substrings{};
    };

}

/******************************************************************************/
/* [Implementation] RockHopper::StringBuilder *********************************/
/******************************************************************************/

template <typename _Tp>
RockHopper::StringBuilder& RockHopper::StringBuilder::operator<<(const _Tp& arg)
{
    m_Substrings.push_back(std::string{arg});
    return *this;
}
