//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include <thread>
#include <boost/test/unit_test.hpp>
#include "sonia/utility/scope_exit.hpp"

#if BOOST_OS_WINDOWS
#include "sonia/sys/windows/thread_pool.hpp"
BOOST_AUTO_TEST_CASE (windows_timer_test)
{
    using namespace sonia;
    using namespace sonia::windows;
    using namespace std::chrono_literals;

    shared_ptr<threadpool> tp = make_shared<threadpool>(1);
    
    std::atomic<int> check = 0;
    auto tmr = tp->create_timer();
    tmr->set(50ms, [&check]{ check |= 1; }); // must be ignored
    tmr->set(60ms, [&check]{ check |= 2; });
    auto tmr2 = tp->create_timer();
    tmr2->set(70ms, [&check]{ check |= 4; }); // must be ignored
    tmr2->disarm();
    {
        auto tmr3 = tp->create_timer();
        tmr3->set(50ms, [&check]{ check |= 8; }); // must be ignored
    }
    tmr2->set(30ms, [&check]{ check |= 16; });
    std::this_thread::sleep_for(100ms);
    BOOST_CHECK_EQUAL(check.load(), 18);
}
#endif

#ifdef __linux__
#include "sonia/sys/linux/timer.hpp"
#include "sonia/sys/linux/signals.hpp"
BOOST_AUTO_TEST_CASE (linux_timer_test)
{
    using namespace sonia;
    using namespace sonia::linux;
    using namespace std::chrono_literals;

    run_watchers(1);
    SCOPE_EXIT([]{ stop_watchers(); });

    std::atomic<int> check = 0;
    timer tmr{ [&check]{ check |= 2; } };
    tmr.set(50ms); // must be ignored
    tmr.set(60ms);
    timer tmr2{ [&check]{ check |= 4; } };
    tmr2.set(70ms); // must be ignored
    tmr2.disarm();
    {
        timer tmr3{ [&check]{ check |= 8; } };
        tmr3.set(50ms); // must be ignored
    }
    tmr2 = timer{ [&check]{ check |= 16; } };
    tmr2.set(30ms);
    std::this_thread::sleep_for(100ms);
    BOOST_CHECK_EQUAL(check.load(), 18);
}
#endif

#ifdef __APPLE__
BOOST_AUTO_TEST_CASE (apple_timer_test)
{
    BOOST_CHECK(false);
}
#endif

#include "applied/scoped_services.hpp"
#include "sonia/services/timer.hpp"
BOOST_AUTO_TEST_CASE (service_timer_test)
{
    using namespace sonia;
    using namespace std::chrono_literals;

    scoped_services ss;

    std::atomic<int> check = 0;
    services::timer tmr{ [&check]{ check |= 2; } };
    tmr.set(50ms); // must be ignored
    tmr.set(60ms);
    services::timer tmr2{ [&check]{ check |= 4; } };
    tmr2.set(70ms); // must be ignored
    tmr2.disarm();
    {
        services::timer tmr3{ [&check]{ check |= 8; } };
        tmr3.set(50ms); // must be ignored
    }
    tmr2 = services::timer{ [&check]{ check |= 16; } };
    tmr2.set(30ms);
    std::this_thread::sleep_for(100ms);
    BOOST_CHECK_EQUAL(check.load(), 18);
}
