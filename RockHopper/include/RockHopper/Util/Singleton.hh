
#ifndef ROCKHOPPER_UTIL_SINGLETON_HH
#define ROCKHOPPER_UTIL_SINGLETON_HH

#include <cstddef>
#include <mutex>
#include <atomic>
#include <type_traits>

namespace RockHopper::Util
{

    class NullSingleton_T {};
    static inline constexpr NullSingleton_T NullSingleton{};

    template <typename T>
    class Singleton
    {
    public:
        virtual ~Singleton();
        explicit Singleton();

        Singleton(Singleton const&);
        Singleton& operator=(Singleton const&);

        Singleton(Singleton&&) noexcept;
        Singleton& operator=(Singleton&&) noexcept;

        Singleton(NullSingleton_T);
        Singleton& operator=(NullSingleton_T);

        friend bool operator==(Singleton const&, NullSingleton_T);
        friend bool operator==(NullSingleton_T, Singleton const&);

        operator bool() const { return m_IsValid; }

        auto valid() const -> bool { return m_IsValid; }
        auto use_count() const -> size_t { return s_UseCount; }

        // UB if `not valid()`
        [[nodiscard]] inline auto operator*() & -> T& { return *s_Pointer; }
        [[nodiscard]] inline auto operator*() const& -> T const& { return *s_Pointer; }
        [[nodiscard]] inline auto operator*() && = delete;
        [[nodiscard]] inline auto operator*() const&& = delete;

        // UB if `not valid()`
        [[nodiscard]] inline auto operator->() & -> T* { return s_Pointer; }
        [[nodiscard]] inline auto operator->() const& -> T const* { return s_Pointer; }
        [[nodiscard]] inline auto operator->() && = delete;
        [[nodiscard]] inline auto operator->() const&& = delete;

        // UB if `not valid()`
        [[nodiscard]] inline auto get() & -> T& { return *s_Pointer; }
        [[nodiscard]] inline auto get() const& -> T const& { return *s_Pointer; }
        [[nodiscard]] inline auto get() && = delete;
        [[nodiscard]] inline auto get() const&& = delete;

    private:
        bool m_IsValid = false;

    private:
        static inline T* s_Pointer = nullptr;
        static inline size_t s_UseCount = 0;
        static inline std::mutex s_Mutex{};

        void AttachThisUse();
        void DetachThisUse();
    };

    template <typename T>
    void Singleton<T>::AttachThisUse()
    {
        std::unique_lock<std::mutex> lock {s_Mutex};

        if (m_IsValid) return;

        if (++s_UseCount == 1)
        {
            s_Pointer = new T;
        }
        m_IsValid = true;
    }

    template <typename T>
    void Singleton<T>::DetachThisUse()
    {
        std::unique_lock<std::mutex> lock {s_Mutex};

        if (not m_IsValid) return;

        if (--s_UseCount == 0)
        {
            delete s_Pointer;
            s_Pointer = nullptr;
        }
        m_IsValid = false;
    }

    template <typename T>
    Singleton<T>::~Singleton()
    {
        DetachThisUse();
    }

    template <typename T>
    Singleton<T>::Singleton()
    {
        static_assert(not std::is_copy_constructible<T>::value);
        static_assert(not std::is_copy_assignable<T>::value);
        static_assert(not std::is_move_constructible<T>::value);
        static_assert(not std::is_move_assignable<T>::value);

        AttachThisUse();
    }

    template <typename T>
    Singleton<T>::Singleton(Singleton const& other)
    {
        *this = other;
    }

    template <typename T>
    Singleton<T>& Singleton<T>::operator=(Singleton const& other)
    {
        if (other.m_IsValid) AttachThisUse();
        return *this;
    }

    template <typename T>
    Singleton<T>::Singleton(Singleton&& other) noexcept
    {
        *this = std::move(other);
    }

    template <typename T>
    Singleton<T>& Singleton<T>::operator=(Singleton&& other) noexcept
    {
        std::swap(m_IsValid,other.m_IsValid);
        return *this;
    }

    template <typename T>
    Singleton<T>::Singleton(NullSingleton_T)
    {
    }

    template <typename T>
    Singleton<T>& Singleton<T>::operator=(NullSingleton_T)
    {
        DetachThisUse();
        return *this;
    }

} // namespace RockHopper::Util

#endif /* ROCKHOPPER_UTIL_SINGLETON_HH */
