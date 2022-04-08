
#ifndef __HH_ROCKHOPPER_UTIL_ASYNC__WAIT_LOCK_
#define __HH_ROCKHOPPER_UTIL_ASYNC__WAIT_LOCK_

#include <memory>
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace RockHopper
{

    class WaitLock
    {
        struct Data
        {
            std::mutex m_Mutex{};
            std::condition_variable m_LockVar{};
            std::atomic<bool> m_IsAlive = true;

            std::atomic<bool> m_Notified = false;
            std::atomic<size_t> m_NumWaiting = 0;
        };

    public:
        virtual ~WaitLock();
        explicit WaitLock();

        explicit WaitLock(WaitLock&& other);
        WaitLock& operator=(WaitLock&& other);

        class Async final
        {
        public:
            explicit Async(std::shared_ptr<Data> const& data_ptr);

            bool alive() noexcept;
            bool wait() noexcept;

        private:
            std::shared_ptr<Data> m_DataPtr;
        };
        auto async() const { return Async{m_DataPtr}; }

    public:
        void notify_one() noexcept;
        void notify_all() noexcept;

        size_t num_waiting() const;

    private:
        std::shared_ptr<Data> m_DataPtr = std::make_shared<Data>();
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_UTIL_ASYNC__WAIT_LOCK_ */
