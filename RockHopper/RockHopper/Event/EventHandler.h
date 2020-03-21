
#pragma once

#include <functional>
#include <unordered_map>
#include <memory>
#include <mutex>

namespace RockHopper
{

    using EventCallbackID = void*;

    template <typename _Tevent>
    class EventHandler
    {
    public:
        using EventCallback = std::function<void(const _Tevent&)>;

        EventCallbackID registerEventCallback(EventCallback&&);
        size_t deregisterEventCallback(EventCallbackID);

        void handleEvent(const _Tevent&);

    private:
        std::mutex m_CallbackMapMutex{};
        std::unordered_map<EventCallbackID,std::unique_ptr<EventCallback>> m_CallbackMap{};
    };

}

/******************************************************************************/
/* [Implementation] RockHopper::EventHandler **********************************/
/******************************************************************************/

template <typename _Tevent>
RockHopper::EventCallbackID
RockHopper::EventHandler<_Tevent>::registerEventCallback(EventCallback&& callback)
{
    auto func = std::make_unique<EventCallback>(std::move(callback));
    auto id = reinterpret_cast<EventCallbackID>(func.get());
    auto mapguard = std::lock_guard{m_CallbackMapMutex};
    m_CallbackMap.insert({id,std::move(func)});
    return id;
}

template <typename _Tevent>
size_t RockHopper::EventHandler<_Tevent>::deregisterEventCallback(EventCallbackID id)
{
    auto mapguard = std::lock_guard{m_CallbackMapMutex};
    return m_CallbackMap.erase(id);
}

template <typename _Tevent>
void RockHopper::EventHandler<_Tevent>::handleEvent(const _Tevent& event)
{
    auto mapguard = std::lock_guard{m_CallbackMapMutex};
    for (auto& [id,callback] : m_CallbackMap) {
        callback->operator()(event);
    }
}
