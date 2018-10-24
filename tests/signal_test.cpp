//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>

#include <signal.h>

//using namespace sonia;
#include <vector>
#include <iostream>
#include "sonia/utility/posix/signals.hpp"

//#include <boost/thread.hpp>

void interuption_handler() {
    std::cout << "interuption_handler is called\n";
}

BOOST_AUTO_TEST_CASE(signal_test)
{
    sonia::posix::set_interruption_handler(&interuption_handler);
    sonia::posix::run_watchers(10);
    sleep(1);
    sonia::posix::raise_user_signal([]() { std::cout << "USER SIGNAL RECEIVED!!!\n"; });
    sleep(3);
    sonia::posix::stop_watchers();
    std::cout << "finish";
}

#if 0
void watcher_proc(int num) {
    int sig;
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGUSR1);
    siginfo_t siginfo;

    int counter = 0;
    while(!sigwaitinfo(&sigset, &siginfo)) {
        std::cout << "signal thread #" << num << " tick " << counter << "\n";
        ++counter;
        if (siginfo.si_signo == SIGUSR1)
        if (counter == 3) break;
        sleep(1);
    }
}

BOOST_AUTO_TEST_CASE (signal_test)
{
    sigset_t new_mask;
    sigfillset(&new_mask);
    sigdelset(&new_mask, SIGSEGV);
    sigdelset(&new_mask, SIGBUS);
    //sigdelset(&new_mask, SIGINT);
    sigset_t old_mask;
    pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

    //if (signal (SIGINT, interuption_handler) == SIG_ERR)
    //    return;

    std::vector<boost::thread> threads;
    threads.emplace_back([]() {
        watcher_proc(0);
    });
    threads.emplace_back([]() {
        watcher_proc(1);
    });

    threads.emplace_back([]() {
        sigset_t new_mask;
        sigfillset(&new_mask);
        sigdelset(&new_mask, SIGINT);
        pthread_sigmask(SIG_BLOCK, &new_mask, nullptr);
        sleep(5);
        std::cout << "thread1 after sleep\n";
    });
    threads.emplace_back([]() {
        sleep(5);
        std::cout << "thread2 after sleep\n";
    });

    /*
    {
        int sig;
        sigset_t sigset;
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGINT);

        while (!sigwait(&sigset, &sig)) {
            printf("mian thread signal\n");
            if (sig == SIGINT) break;
        }
    }
    */
    sleep(5);
    std::cout << "after sleep\n";
    for (auto & t : threads) {
        t.join();
    }
    std::cout << "threads were joined\n";
}

#endif