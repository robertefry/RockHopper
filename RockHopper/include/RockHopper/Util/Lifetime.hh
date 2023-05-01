
#ifndef ROCKHOPPER_UTIL_LIFETIMES_HH
#define ROCKHOPPER_UTIL_LIFETIMES_HH

namespace RockHopper::Util
{

    struct NoCopy
    {
        ~NoCopy() = default;
        NoCopy() = default;

        NoCopy(NoCopy const&) = delete;
        NoCopy& operator=(NoCopy const&) = delete;

        NoCopy(NoCopy&&) noexcept = default;
        NoCopy& operator=(NoCopy&&) noexcept = default;
    };

    struct NoMove
    {
        ~NoMove() = default;
        NoMove() = default;

        NoMove(NoMove const&) = delete;
        NoMove& operator=(NoMove const&) = delete;

        NoMove(NoMove&&) noexcept = delete;
        NoMove& operator=(NoMove&&) noexcept = delete;
    };

} // namespace RockHopper::Util

#endif /* ROCKHOPPER_UTIL_LIFETIMES_HH */
