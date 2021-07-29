
#include "RockHopper/Engine/Timing.hh"

namespace RockHopper
{

    TimeSpan::TimeSpan(double seconds)
        : m_TimeBase{std::chrono::duration_cast<Base>(std::chrono::duration<double>{seconds})}
    {
    }

    bool operator==(TimeSpan const& ts1, TimeSpan const& ts2)
    {
        return ts1.m_TimeBase == ts2.m_TimeBase;
    }

    auto operator<=>(TimeSpan const& ts1, TimeSpan const& ts2)
        -> std::strong_ordering
    {
        return ts1.m_TimeBase.count() <=> ts2.m_TimeBase.count();
    }

    TimeSpan& TimeSpan::operator++()
    {
        ++m_TimeBase;
        return *this;
    }

    TimeSpan& TimeSpan::operator--()
    {
        --m_TimeBase;
        return *this;
    }

    TimeSpan& TimeSpan::operator+=(TimeSpan const& other)
    {
        m_TimeBase += other.m_TimeBase;
        return *this;
    }

    TimeSpan& TimeSpan::operator-=(TimeSpan const& other)
    {
        m_TimeBase -= other.m_TimeBase;
        return *this;
    }

    TimeSpan& TimeSpan::operator*=(TimeSpan const& other)
    {
        m_TimeBase *= other.count();
        return *this;
    }

    TimeSpan& TimeSpan::operator/=(TimeSpan const& other)
    {
        m_TimeBase /= other.count();
        return *this;
    }

    TimeSpan& TimeSpan::operator%=(TimeSpan const& other)
    {
        m_TimeBase %= other.m_TimeBase;
        return *this;
    }

    TimeSpan& TimeSpan::operator*=(double value)
    {
        m_TimeBase *= value;
        return *this;
    }

    TimeSpan& TimeSpan::operator/=(double value)
    {
        m_TimeBase /= value;
        return *this;
    }

    TimeSpan& TimeSpan::operator%=(double value)
    {
        m_TimeBase %= value;
        return *this;
    }

    TimeSpan operator++(TimeSpan& ts1, int)
    {
        auto ts2 = ts1;
        ++ts1;
        return ts2;
    }

    TimeSpan operator--(TimeSpan& ts1, int)
    {
        auto ts2 = ts1;
        --ts1;
        return ts2;
    }

    TimeSpan operator+(TimeSpan const& ts1, TimeSpan const& ts2)
    {
        return TimeSpan{ts1} += ts2;
    }

    TimeSpan operator-(TimeSpan const& ts1, TimeSpan const& ts2)
    {
        return TimeSpan{ts1} -= ts2;
    }

    TimeSpan operator*(TimeSpan const& ts1, TimeSpan const& ts2)
    {
        return TimeSpan{ts1} *= ts2;
    }

    TimeSpan operator/(TimeSpan const& ts1, TimeSpan const& ts2)
    {
        return TimeSpan{ts1} /= ts2;
    }

    TimeSpan operator%(TimeSpan const& ts1, TimeSpan const& ts2)
    {
        return TimeSpan{ts1} %= ts2;
    }

    TimeSpan operator*(TimeSpan const& ts1, double ts2)
    {
        return TimeSpan{ts1} *= ts2;
    }

    TimeSpan operator/(TimeSpan const& ts1, double ts2)
    {
        return TimeSpan{ts1} /= ts2;
    }

    TimeSpan operator%(TimeSpan const& ts1, double ts2)
    {
        return TimeSpan{ts1} %= ts2;
    }

    TimeSpan operator*(double ts2, TimeSpan const& ts1)
    {
        return TimeSpan{ts1} *= ts2;
    }

} // namespace RockHopper
