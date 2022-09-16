
#include "RockHopper/UI/Window.hh"
using namespace RockHopper;

#include "RockHopper/Event/EventListener.hh"
#include "RockHopper/Debug.hh"

#include <thread-tools/wait_lock.hh>
#include <gtest/gtest.h>

template <typename t_type, typename... t_args>
static auto make_ready_future(t_args&&... val) -> std::future<t_type>
{
    std::promise<t_type> promise;
    promise.set_value(std::forward<t_args>(val)...);
    return promise.get_future();
}

struct TestWindow
    : public UI::Window
{
    unsigned event_processed_init = 0;
    unsigned event_processed_dispose = 0;
    unsigned event_processed_refresh = 0;

public:
    virtual ~TestWindow()
    {
    }
    explicit TestWindow(
        std::shared_ptr<Chrono::I_TickThread> const& tick_thread,
        std::shared_ptr<GX::Renderer> const& renderer
    )
        : UI::Window{tick_thread,renderer}
    {
    }

public:
    virtual bool is_visible() const override { return false; };
    virtual auto show() -> std::future<void> override { return make_ready_future<void>(); };
    virtual auto hide() -> std::future<void> override { return make_ready_future<void>(); };

    virtual auto get_width() const -> width_t override { return 0; };
    virtual auto set_width(width_t const&) -> std::future<void> override { return make_ready_future<void>(); };

    virtual auto get_height() const -> height_t override { return 0; };
    virtual auto set_height(height_t const&) -> std::future<void> override { return make_ready_future<void>(); };

    virtual auto get_title() const -> title_t override { return ""; };
    virtual auto set_title(title_t const&) -> std::future<void> override { return make_ready_future<void>(); };

    virtual auto get_swap_interval() const -> swap_interval_t override { return 0; };
    virtual auto set_swap_interval(swap_interval_t const&) -> std::future<void> override { return make_ready_future<void>(); };

private:
    virtual void on_event(UI::WindowEvent_Init const&) override
    {
        ROCKHOPPER_LOG_INFO(__PRETTY_FUNCTION__);
        ++event_processed_init;
    }
    virtual void on_event(UI::WindowEvent_Dispose const&) override
    {
        ROCKHOPPER_LOG_INFO(__PRETTY_FUNCTION__);
        ++event_processed_dispose;
    }
    virtual void on_event(UI::WindowEvent_Refresh const&) override
    {
        ROCKHOPPER_LOG_INFO(__PRETTY_FUNCTION__);
        ++event_processed_refresh;
    }
};

TEST(UI,WINDOW)
{
    using TickThread = Chrono::TickThread<Event::Dispatch::SequentialDispatch>;
    std::shared_ptr<TickThread> tick_thread = std::make_shared<TickThread>();
    std::shared_ptr<GX::Renderer> renderer = std::make_shared<GX::Renderer>();

    TestWindow window {tick_thread,renderer};

    tick_thread->start();

    window.insert_event_listener(
        Event::MakeListener<UI::WindowEvent_Refresh>([&](auto)
    {
        tick_thread->stop();
    }));

    thd::wait_lock wait_on_close;
    window.insert_event_listener(
        Event::MakeListener<UI::WindowEvent_Dispose>([&](auto)
    {
        wait_on_close.notify_all();
    }));
    wait_on_close.wait();

    EXPECT_EQ(window.event_processed_init,1);
    EXPECT_EQ(window.event_processed_dispose,1);
    EXPECT_EQ(window.event_processed_refresh,1);
}
