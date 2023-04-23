
#ifndef ROCKHOPPER_UTIL_SINGLETON_HH
#define ROCKHOPPER_UTIL_SINGLETON_HH

#include <cstddef>
#include <type_traits>

namespace RockHopper::Util
{

    template <typename T>
    class Singleton
    {
        static_assert(not std::is_copy_constructible<T>::value);
        static_assert(not std::is_copy_assignable<T>::value);
        static_assert(not std::is_move_constructible<T>::value);
        static_assert(not std::is_move_assignable<T>::value);

    public:
        virtual ~Singleton();
        explicit Singleton();

        Singleton(Singleton const&);
        Singleton& operator=(Singleton const&);

        Singleton(Singleton&&) noexcept;
        Singleton& operator=(Singleton&&) noexcept;

        [[nodiscard]] inline auto operator*() & -> T& { return *s_Pointer; }
        [[nodiscard]] inline auto operator*() const& -> T const& { return *s_Pointer; }
        [[nodiscard]] inline auto operator*() && = delete;
        [[nodiscard]] inline auto operator*() const&& = delete;

        [[nodiscard]] inline auto operator->() & -> T* { return s_Pointer; }
        [[nodiscard]] inline auto operator->() const& -> T const* { return s_Pointer; }
        [[nodiscard]] inline auto operator->() && = delete;
        [[nodiscard]] inline auto operator->() const&& = delete;

        [[nodiscard]] inline auto get() & -> T& { return *s_Pointer; }
        [[nodiscard]] inline auto get() const& -> T const& { return *s_Pointer; }
        [[nodiscard]] inline auto get() && = delete;
        [[nodiscard]] inline auto get() const&& = delete;

    private:
        static inline T* s_Pointer = nullptr;
        static inline size_t s_UseCount = 0;
    };

    template <typename T>
    Singleton<T>::~Singleton()
    {
        if (--s_UseCount == 0) delete s_Pointer;
    }

    template <typename T>
    Singleton<T>::Singleton()
    {
        if (++s_UseCount == 1) s_Pointer = new T;
    }

    template <typename T>
    Singleton<T>::Singleton(Singleton const&)
    {
        ++s_UseCount;
    }

    template <typename T>
    Singleton<T>& Singleton<T>::operator=(Singleton const&)
    {
        return *this;
    }

    template <typename T>
    Singleton<T>::Singleton(Singleton&&) noexcept
    {
        ++s_UseCount;
    }

    template <typename T>
    Singleton<T>& Singleton<T>::operator=(Singleton&&) noexcept
    {
        return *this;
    }

} // namespace RockHopper::Util

#endif /* ROCKHOPPER_UTIL_SINGLETON_HH */
