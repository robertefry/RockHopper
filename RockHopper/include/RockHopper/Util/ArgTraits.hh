
#ifndef ROCKHOPPER_UTIL_ARG_TRAITS_HH
#define ROCKHOPPER_UTIL_ARG_TRAITS_HH

#include <type_traits>

namespace RockHopper::Util
{

    template <typename T>
    static inline constexpr bool PreferPassByValue = sizeof(T) < 2 * sizeof(void*);

    template <typename T>
    using In = std::conditional_t<PreferPassByValue<T>,T const,T const&>;

} // namespace RockHopper::Util

#endif /* ROCKHOPPER_UTIL_ARG_TRAITS_HH */
