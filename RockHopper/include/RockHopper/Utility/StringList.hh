
#pragma once

#include <list>
#include <string>
#include <sstream>
#include <iosfwd>

/* ************************************************************************** */
// [Definition] RockHopper::StringList
/* ************************************************************************** */

namespace RockHopper
{

    template <typename C = char>
    class StringList
    {
    public:
        explicit StringList() = default;
        virtual ~StringList() = default;

    public:
        StringList<C>& push_back(std::basic_string<C> const&);
        StringList<C>& push_back(std::basic_string<C>&&);
        StringList<C>& push_back(StringList<C> const&);
        StringList<C>& push_back(StringList<C>&&);

        template <typename Tp>
        StringList<C>& operator<<(Tp const&);

        std::basic_string<C> to_string(std::basic_string<C> const& delim = " ") const;

    private:
        std::basic_string<C> m_Delim{};
        std::list<std::basic_string<C>> m_List{};
    };

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::StringList
/* ************************************************************************** */

namespace RockHopper
{

    template <typename C>
    StringList<C>& StringList<C>::push_back(std::basic_string<C> const& string)
    {
        m_List.push_back(string);
        return *this;
    }

    template <typename C>
    StringList<C>& StringList<C>::push_back(std::basic_string<C>&& string)
    {
        m_List.push_back(std::move(string));
        return *this;
    }

    template <typename C>
    StringList<C>& StringList<C>::push_back(StringList<C> const& stringlist)
    {
        for (std::basic_string<C> const& string : stringlist) {
            m_List.push_back(string);
        }
        return *this;
    }

    template <typename C>
    StringList<C>& StringList<C>::push_back(StringList<C>&& stringlist)
    {
        for (std::basic_string<C>&& string : stringlist) {
            m_List.push_back(std::move(string));
        }
        return *this;
    }

    template <typename C>
    template <typename Tp>
    StringList<C>& StringList<C>::operator<<(Tp const& obj)
    {
        std::basic_ostringstream<C> oss;
        oss << obj;
        push_back(oss.str());
        return *this;
    }

    template <typename C>
    std::basic_string<C> StringList<C>::to_string(std::basic_string<C> const& delim) const
    {
        size_t size = 0;
        for (auto const& string : m_List) {
            size += string.size();
        }
        if (m_List.size()) size += delim.size() * (m_List.size()-1);

        std::basic_string<C> result;
        result.reserve(size);

        if (m_List.size())
        {
            auto itr = m_List.begin();
            result += *itr;

            for (++itr; itr != m_List.end(); ++itr)
            {
                result += delim;
                result += *itr;
            }
        }

        return result;
    }

} // namespace RockHopper

/* ************************************************************************** */
//  Standard Extensions
/* ************************************************************************** */

namespace std
{

    template <typename C = char>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::StringList<C> const& strl)
    {
        return ost << strl.to_string();
    }

} // namespace std
