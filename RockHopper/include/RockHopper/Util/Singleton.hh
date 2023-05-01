
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

        Singleton(NullSingleton_T);
        Singleton& operator=(NullSingleton_T);

        friend bool operator==(Singleton const&, NullSingleton_T);
        friend bool operator==(NullSingleton_T, Singleton const&);

        Singleton(Singleton const&);
        Singleton& operator=(Singleton const&);

        Singleton(Singleton&&) noexcept;
        Singleton& operator=(Singleton&&) noexcept;

        operator bool() const { return s_UseCount != 0; }
        auto use_count() const -> size_t { return s_UseCount; }

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
        static inline std::atomic<size_t> s_UseCount = 0;

    private:
        static inline std::mutex s_Mutex{};

        void use_one_more();
        void use_one_less();
    };

    template <typename T>
    void Singleton<T>::use_one_more()
    {
        std::unique_lock<std::mutex> lock {s_Mutex};

        if (++s_UseCount == 1) s_Pointer = new T;
    }

    template <typename T>
    void Singleton<T>::use_one_less()
    {
        std::unique_lock<std::mutex> lock {s_Mutex};

        if (s_UseCount == 0) return;
        if (--s_UseCount == 0) delete s_Pointer;
    }

    template <typename T>
    Singleton<T>::~Singleton()
    {
        use_one_less();
    }

    template <typename T>
    Singleton<T>::Singleton()
    {
        static_assert(not std::is_copy_constructible<T>::value);
        static_assert(not std::is_copy_assignable<T>::value);
        static_assert(not std::is_move_constructible<T>::value);
        static_assert(not std::is_move_assignable<T>::value);

        use_one_more();
    }

    template <typename T>
    Singleton<T>::Singleton(NullSingleton_T)
    {
    }

    template <typename T>
    Singleton<T>& Singleton<T>::operator=(NullSingleton_T)
    {
        use_one_less();
    }

    template <typename T>
    Singleton<T>::Singleton(Singleton const&)
    {
        use_one_more();
    }

    template <typename T>
    Singleton<T>& Singleton<T>::operator=(Singleton const&)
    {
        return *this;
    }

    template <typename T>
    Singleton<T>::Singleton(Singleton&&) noexcept
    {
        use_one_more();
    }

    template <typename T>
    Singleton<T>& Singleton<T>::operator=(Singleton&&) noexcept
    {
        return *this;
    }

} // namespace RockHopper::Util

#endif /* ROCKHOPPER_UTIL_SINGLETON_HH */
