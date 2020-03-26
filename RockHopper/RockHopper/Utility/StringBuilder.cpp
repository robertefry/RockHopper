
#include "StringBuilder.h"

#include <cstring>

RockHopper::StringBuilder::StringBuilder()
{
}

RockHopper::StringBuilder::~StringBuilder()
{
}

RockHopper::StringBuilder::StringBuilder(const StringBuilder& other)
    : m_Substrings{other.m_Substrings}
{
}

RockHopper::StringBuilder::StringBuilder(StringBuilder&& other)
    : m_Substrings{std::move(other.m_Substrings)}
{
}

RockHopper::StringBuilder& RockHopper::StringBuilder::operator=(const StringBuilder& other)
{
    m_Substrings = other.m_Substrings;
    return *this;
}

RockHopper::StringBuilder& RockHopper::StringBuilder::operator=(StringBuilder&& other)
{
    m_Substrings = std::move(other.m_Substrings);
    return *this;
}

size_t RockHopper::StringBuilder::buckets() const
{
    return m_Substrings.size();
}

size_t RockHopper::StringBuilder::size() const
{
    size_t count = 0;
    for (const auto& str : m_Substrings) {
        count += str.size();
    }
    return count;
}

std::string RockHopper::StringBuilder::str() const
{
    char* buffer = new char[size()+1];
    buffer[0] = '\0';
    for (const auto& str : m_Substrings) {
        strncat(buffer, str.c_str(), str.size());
    }
    std::string result{buffer};
    delete[] buffer;
    return result;
}

void RockHopper::StringBuilder::clear()
{
    m_Substrings.clear();
}
