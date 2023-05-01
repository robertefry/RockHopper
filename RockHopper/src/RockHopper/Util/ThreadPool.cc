
#include "RockHopper/Util/ThreadPool.hh"

namespace RockHopper::Util
{

    static auto const s_HardwareConcurrency = []() noexcept
    {
        auto hardware_concurrency = std::thread::hardware_concurrency();
        return hardware_concurrency == 0 ? 1 : hardware_concurrency;
    }();

    ThreadPool::Impl::~Impl()
    {
        m_IsStopRequested = true;

        for (auto& thread : m_ThreadList)
        {
            if (thread.joinable()) thread.join();
        }
    }

    ThreadPool::Impl::Impl()
        : Util::TaskQueue{s_HardwareConcurrency}
    {
        m_ThreadList.reserve(s_HardwareConcurrency);

        for (size_t i = 0; i < s_HardwareConcurrency; ++i)
        {
            m_ThreadList.emplace_back([&]{ this->run(); });
        }
    }

    void ThreadPool::Impl::run()
    {
        while (not m_IsStopRequested)
        {
            if (not empty()) execute_one();
        }
    }

} // namespace RockHopper::Util
