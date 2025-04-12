
#ifndef __HH_ROCKHOPPER_UTIL_TIME_TIMESPAN_
#define __HH_ROCKHOPPER_UTIL_TIME_TIMESPAN_

#include <cstdint>
#include <compare>
#include <chrono>

namespace RockHopper
{

    class TimeSpan
    {
    public:
        using Base = std::chrono::duration<uint64_t,std::nano>;

        virtual ~TimeSpan() = default;
        explicit TimeSpan() = default;

        TimeSpan(double seconds);

        template <typename T_Rep, typename T_Period>
        TimeSpan(std::chrono::duration<T_Rep,T_Period> const& rtime)
            : m_TimeBase{std::chrono::duration_cast<Base>(rtime)}
        {}

        auto duration() const { return m_TimeBase; }

        template <typename T_Rep = double, typename T_Period = std::ratio<1,1>>
        auto count() const
        {
            using target = std::chrono::duration<T_Rep,T_Period>;
            return std::chrono::duration_cast<target>(m_TimeBase).count();
        }

        operator double() const { return count(); }

        friend bool operator==(TimeSpan const&, TimeSpan const&);
        friend auto operator<=>(TimeSpan const&, TimeSpan const&)
            -> std::strong_ordering;

        TimeSpan& operator++();
        TimeSpan& operator--();

        TimeSpan& operator+=(TimeSpan const&);
        TimeSpan& operator-=(TimeSpan const&);
        TimeSpan& operator*=(TimeSpan const&);
        TimeSpan& operator/=(TimeSpan const&);
        TimeSpan& operator%=(TimeSpan const&);

        TimeSpan& operator*=(double);
        TimeSpan& operator/=(double);
        TimeSpan& operator%=(double);

    private:
        Base m_TimeBase{};
    };

    TimeSpan operator++(TimeSpan&, int);
    TimeSpan operator--(TimeSpan&, int);

    TimeSpan operator+(TimeSpan const&, TimeSpan const&);
    TimeSpan operator-(TimeSpan const&, TimeSpan const&);
    TimeSpan operator*(TimeSpan const&, TimeSpan const&);
    TimeSpan operator/(TimeSpan const&, TimeSpan const&);
    TimeSpan operator%(TimeSpan const&, TimeSpan const&);

    TimeSpan operator*(TimeSpan const&, double);
    TimeSpan operator/(TimeSpan const&, double);
    TimeSpan operator%(TimeSpan const&, double);

    TimeSpan operator*(double, TimeSpan const&);

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_UTIL_TIME_TIMESPAN_ */
