
#ifndef ROCKHOPPER_UTIL_FUTURE_HH
#define ROCKHOPPER_UTIL_FUTURE_HH

#include <future>
#include <atomic>

namespace RockHopper::Util
{

    template <typename T>
    class Future final
    {
    public:
        ~Future() = default;
        explicit Future() noexcept = default;

        Future(Future&&) noexcept = default;
        Future& operator=(Future&&) noexcept = default;

        Future(std::future<T>&& future) noexcept;
        operator std::future<T> &&() && noexcept;
        operator std::future<T> const&&() const&& noexcept;

        [[nodiscard]] auto status() const -> std::future_status;
        [[nodiscard]] bool valid() const noexcept;

        [[nodiscard]] auto get() { return m_Future.get(); }

        void wait() const;
        void spin() const;

        template <typename T_Clock = std::chrono::system_clock, typename T_Duration>
        [[nodiscard]] auto wait_for(T_Duration const&) const -> std::future_status;
        template <typename T_Clock = std::chrono::system_clock, typename T_Duration>
        [[nodiscard]] auto spin_for(T_Duration const&) const -> std::future_status;

        template <typename T_Clock = std::chrono::system_clock, typename T_TimePoint>
        [[nodiscard]] auto wait_until(T_TimePoint const&) const -> std::future_status;
        template <typename T_Clock = std::chrono::system_clock, typename T_TimePoint>
        [[nodiscard]] auto spin_until(T_TimePoint const&) const -> std::future_status;

    private:
        std::future<T> m_Future;
    };

    template <typename T>
    Future<T>::Future(std::future<T>&& future) noexcept
        : m_Future{std::move(future)}
    {
    }

    template <typename T>
    Future<T>::operator std::future<T> &&() && noexcept
    {
        return std::move(m_Future);
    }

    template <typename T>
    Future<T>::operator std::future<T> const&&() const&& noexcept
    {
        return std::move(m_Future);
    }

    template <typename T>
    auto Future<T>::status() const -> std::future_status
    {
        return m_Future.wait_for(std::chrono::nanoseconds{0});
    }

    template <typename T>
    bool Future<T>::valid() const noexcept
    {
        return m_Future.valid();
    }

    template <typename T>
    void Future<T>::wait() const
    {
        m_Future.wait();
    }

    template <typename T>
    void Future<T>::spin() const
    {
        do { /* spin */ } while (status() != std::future_status::ready);
    }

    template <typename T>
    template <typename T_Clock, typename T_Duration>
    auto Future<T>::wait_for(T_Duration const& duration) const -> std::future_status
    {
        return m_Future.wait_for(duration);
    }

    template <typename T>
    template <typename T_Clock, typename T_Duration>
    auto Future<T>::spin_for(T_Duration const& duration) const -> std::future_status
    {
        auto const time_point = T_Clock::now() + duration;
        return spin_until<T_Clock>(time_point);
    }

    template <typename T>
    template <typename T_Clock, typename T_TimePoint>
    auto Future<T>::wait_until(T_TimePoint const& time_point) const -> std::future_status
    {
        return m_Future.wait_until(time_point);
    }

    template <typename T>
    template <typename T_Clock, typename T_TimePoint>
    auto Future<T>::spin_until(T_TimePoint const& time_point) const -> std::future_status
    {
        auto status = this->status();

        while (T_Clock::now() < time_point)
        {
            if (status == std::future_status::ready) break;
            status = this->status();
        }
        return status;
    }

    template <typename T>
    class SharedFuture final
    {
    public:
        ~SharedFuture() = default;
        explicit SharedFuture() noexcept = default;

        SharedFuture(SharedFuture<T> const&) noexcept = default;
        SharedFuture& operator=(SharedFuture<T> const&) noexcept = default;

        SharedFuture(SharedFuture&&) noexcept = default;
        SharedFuture& operator=(SharedFuture&&) noexcept = default;

        explicit SharedFuture(std::future<T>&&) noexcept;
        explicit SharedFuture(Future<T>&&) noexcept;

        SharedFuture(std::shared_future<T> const&) noexcept;
        operator std::shared_future<T> &() & noexcept;
        operator std::shared_future<T> const&() const& noexcept;
        operator std::shared_future<T> &&() && noexcept;
        operator std::shared_future<T> const&&() const&& noexcept;

        [[nodiscard]] auto status() const -> std::future_status;
        [[nodiscard]] bool valid() const noexcept;

        [[nodiscard]] auto get() { return m_SharedFuture.get(); }

        void wait() const;
        void spin() const;

        template <typename T_Clock = std::chrono::system_clock, typename T_Duration>
        [[nodiscard]] auto wait_for(T_Duration const&) const -> std::future_status;
        template <typename T_Clock = std::chrono::system_clock, typename T_Duration>
        [[nodiscard]] auto spin_for(T_Duration const&) const -> std::future_status;

        template <typename T_Clock = std::chrono::system_clock, typename T_TimePoint>
        [[nodiscard]] auto wait_until(T_TimePoint const&) const -> std::future_status;
        template <typename T_Clock = std::chrono::system_clock, typename T_TimePoint>
        [[nodiscard]] auto spin_until(T_TimePoint const&) const -> std::future_status;

    private:
        std::shared_future<T> m_SharedFuture;
    };

    template <typename T>
    SharedFuture<T>::SharedFuture(std::future<T>&& future) noexcept
        : m_SharedFuture{std::move(future)}
    {
    }

    template <typename T>
    SharedFuture<T>::SharedFuture(Future<T>&& future) noexcept
        : m_SharedFuture{static_cast<std::future<T>&&>(std::move(future))}
    {
    }

    template <typename T>
    SharedFuture<T>::SharedFuture(std::shared_future<T> const& future) noexcept
        : m_SharedFuture{future}
    {
    }

    template <typename T>
    SharedFuture<T>::operator std::shared_future<T> &() & noexcept
    {
        return m_SharedFuture;
    }

    template <typename T>
    SharedFuture<T>::operator std::shared_future<T> const&() const& noexcept
    {
        return m_SharedFuture;
    }

    template <typename T>
    SharedFuture<T>::operator std::shared_future<T> &&() && noexcept
    {
        return std::move(m_SharedFuture);
    }

    template <typename T>
    SharedFuture<T>::operator std::shared_future<T> const&&() const&& noexcept
    {
        return std::move(m_SharedFuture);
    }

    template <typename T>
    auto SharedFuture<T>::status() const -> std::future_status
    {
        return m_SharedFuture.wait_for(std::chrono::nanoseconds{0});
    }

    template <typename T>
    bool SharedFuture<T>::valid() const noexcept
    {
        return m_SharedFuture.valid();
    }

    template <typename T>
    void SharedFuture<T>::wait() const
    {
        m_SharedFuture.wait();
    }

    template <typename T>
    void SharedFuture<T>::spin() const
    {
        do { /* spin */ } while (status() != std::future_status::ready);
    }

    template <typename T>
    template <typename T_Clock, typename T_Duration>
    auto SharedFuture<T>::wait_for(T_Duration const& duration) const -> std::future_status
    {
        return m_SharedFuture.wait_for(duration);
    }

    template <typename T>
    template <typename T_Clock, typename T_Duration>
    auto SharedFuture<T>::spin_for(T_Duration const& duration) const -> std::future_status
    {
        auto const time_point = T_Clock::now() + duration;
        return spin_until<T_Clock>(time_point);
    }

    template <typename T>
    template <typename T_Clock, typename T_TimePoint>
    auto SharedFuture<T>::wait_until(T_TimePoint const& time_point) const -> std::future_status
    {
        return m_SharedFuture.wait_until(time_point);
    }

    template <typename T>
    template <typename T_Clock, typename T_TimePoint>
    auto SharedFuture<T>::spin_until(T_TimePoint const& time_point) const -> std::future_status
    {
        auto status = this->status();

        while (T_Clock::now() < time_point)
        {
            if (status == std::future_status::ready) break;
            status = this->status();
        }
        return status;
    }

} // namespace RockHopper::Util

#endif /* ROCKHOPPER_UTIL_FUTURE_HH */
