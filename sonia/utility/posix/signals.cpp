//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "signals.hpp"

#include <boost/exception/diagnostic_information.hpp>

#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/thread.hpp"
#include "sonia/logger/logger.hpp"
#include "sonia/utility/scope_exit.hpp"

#include <signal.h>
#include <pthread.h>
#include <atomic>

namespace sonia { namespace posix {

int user_signal_ = SIGRTMIN;
user_handler_type interruption_handler_;

std::atomic<void*> watchers_ {nullptr};

void set_user_signal(int val) {
    user_signal_ = val;
}

void set_interruption_handler(user_handler_type const& ih) {
    interruption_handler_ = ih;
}

void watcher_proc(int num)
{
    std::cout << ("start signal watcher %1%\n"_fmt % num).str();

    sigset_t sigset;
    sigfillset(&sigset);
    pthread_sigmask(SIG_BLOCK, &sigset, nullptr);

    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, user_signal_);
    siginfo_t siginfo;

    for (;;) {
        int sig = sigwaitinfo(&sigset, &siginfo);
        if (-1 == sig) {
            int err = errno;
            if (EINTR == err) continue;
            GLOBAL_LOG_ERROR() << "watcher %1%, sigwaitinfo error: %2%"_fmt % num % strerror(err);
            continue;
        }
        std::cout << ("watcher %1% received a signal %2%\n"_fmt % num % sig).str();
        
        try {
            if (sig == user_signal_) {
                int code = siginfo.si_code;
                if (SI_USER == code) break;
                if (SI_QUEUE == code || SI_ASYNCIO == code) {
                    if (!siginfo.si_value.sival_ptr) break;
                    user_handler_type * pfunc = reinterpret_cast<user_handler_type*>(siginfo.si_value.sival_ptr);
                    (*pfunc)();
                }
            } else if (sig == SIGINT) {
                if (interruption_handler_) {
                    interruption_handler_();
                }
            }
            // else ignore
        } catch (std::exception const& e) {
            GLOBAL_LOG_ERROR() << boost::diagnostic_information(e);
        } catch (...) {
            GLOBAL_LOG_ERROR() << boost::current_exception_diagnostic_information();
        }
    }

    std::cout << ("stop signal watcher %1%\n"_fmt % num).str();
}

void raise_user_signal(user_handler_type const& pfunc)
{
    sigval val;
    val.sival_ptr = (void*)&pfunc;
    int counter = 10;
    for (;;) {
        int r = sigqueue(getpid(), user_signal_, val);
        if (!r) return;
        if (EAGAIN == r && counter >= 0) {
            sleep(1);
            --counter;
        } else {
            int err = errno;
            throw exception("can't raise user signal, error: "_fmt % strerror(err));
        }
    }
}

void run_watchers(int count)
{
    sigset_t new_mask;
    sigfillset(&new_mask);
    sigdelset(&new_mask, SIGSEGV);
    sigdelset(&new_mask, SIGBUS);
    //sigdelset(&new_mask, SIGINT);
    //sigdelset(&new_mask, user_signal_);
    pthread_sigmask(SIG_BLOCK, &new_mask, nullptr);
    //signal (SIGINT, SIG_IGN);

    std::vector<thread> *wptr = new std::vector<thread> {};
    SCOPE_EXIT([&wptr]() {
        if (wptr) delete wptr;
    });
    wptr->reserve(count);
    void* exp = nullptr;
    if (!watchers_.compare_exchange_strong(exp, wptr)) {
        return;
    }
    std::vector<thread> & ws = *wptr;
    wptr = nullptr; // unpin
    for (int c = 0; c < count; ++c) {
        ws.emplace_back([c] { watcher_proc(c); });
    }
}

void stop_watchers()
{
    void* exp = watchers_.load();
    if (!exp) return;
    if (!watchers_.compare_exchange_strong(exp, nullptr)) {
        return;
    }

    std::vector<thread> *wptr = reinterpret_cast<std::vector<thread>*>(exp);
    
    sigval val;
    val.sival_ptr = nullptr;

    for (size_t c = 0; c < wptr->size(); ++c) {
        std::cout << ("raising signal %1% num %2%\n"_fmt % user_signal_ % c).str();
        if (sigqueue(getpid(), user_signal_, val)) {
            int err = errno;
            GLOBAL_LOG_ERROR() << "while stop_watchers : can't raise user signal, error: " << strerror(err);
            // detach threads
            for (thread & t : *wptr) {
                t.detach();
            }
            delete wptr;
            return;
        }
    }

    for (thread & t : *wptr) {
        t.join();
    }

    delete wptr;
}

}}
