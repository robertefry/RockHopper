
#include "RockHopper/Utility/Async/WaitLock.hh"

namespace RockHopper
{

    WaitLock::~WaitLock()
    {
        m_DataPtr->m_IsAlive = false;
        notify_all();
    }

    WaitLock::WaitLock()
    {
    }

    void WaitLock::notify_one() noexcept
    {
        m_DataPtr->m_LockVar.notify_one();
    }

    void WaitLock::notify_all() noexcept
    {
        m_DataPtr->m_Notified = true;
        m_DataPtr->m_LockVar.notify_all();

        while (m_DataPtr->m_NumWaiting != 0)
        {
            // spin while waiting threads wake up
        }
        m_DataPtr->m_Notified = false;
    }

    WaitLock::Async::Async(std::shared_ptr<Data> const& data_ptr)
        : m_DataPtr{data_ptr}
    {
    }

    bool WaitLock::Async::alive() noexcept
    {
        return m_DataPtr->m_IsAlive;
    }

    bool WaitLock::Async::wait() noexcept
    {
        if (not m_DataPtr->m_IsAlive) return false;

        bool notified = m_DataPtr->m_Notified;
        if (not notified)
        {
            std::unique_lock lock {m_DataPtr->m_Mutex};

            m_DataPtr->m_NumWaiting += 1;
            m_DataPtr->m_LockVar.wait(lock);
            notified = m_DataPtr->m_Notified; // check for spurious wakeup
            m_DataPtr->m_NumWaiting -= 1;
        }
        return notified;
    }

} // namespace RockHopper
