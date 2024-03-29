//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "signals.hpp"

#include <thread>
#include <atomic>
#include <boost/unordered_map.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/logger/logger.hpp"
#include "sonia/utility/scope_exit.hpp"

#include <signal.h>
#include <pthread.h>
#include <atomic>

namespace sonia::linux {

static const bool SIGNAL_DEBUG = true;

int user_signal_ = SIGRTMIN;

user_handler_type interruption_handler_;

struct watch_descriptor
{
    std::vector<thread> threads;
    boost::unordered_map<int, shared_ptr<handler_entry>> entries;
    spin_mutex entries_mtx;
    std::atomic<intptr_t> ecounter{1};
    
    intptr_t new_entry(shared_ptr<handler_entry> e)
    {
        intptr_t val = ecounter.fetch_add(2);
        lock_guard guard(entries_mtx);
        BOOST_VERIFY (entries.insert(std::pair(val, std::move(e))).second);
        return val;
    }

    shared_ptr<handler_entry> get_entry(intptr_t val)
    {
        lock_guard guard(entries_mtx);
        auto it = entries.find(val);
        return it != entries.end() ? it->second : shared_ptr<handler_entry>{};
    }

    void delete_entry(intptr_t val)
    {
        lock_guard guard(entries_mtx);
        entries.erase(val);
    }

    ~watch_descriptor()
    {
        if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << "~watch_descriptor";
        {
            lock_guard guard(entries_mtx);
            if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << "watch_descriptor : entry count: " << entries.size();
            entries.clear();
        }

        if (threads.empty()) return;

        sigval val;
        val.sival_ptr = 0;

        for (size_t c = 0; c < threads.size(); ++c) {
            if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << ("raising stop watcher signal %1%(%2%)"_fmt % user_signal_ % c).str();
            if (sigqueue(getpid(), user_signal_, val)) {
                int err = errno;
                GLOBAL_LOG_ERROR() << "while stop_watchers : can't raise user signal, error: " << strerror(err);
                // detach threads
                for (thread & t : threads) {
                    t.detach();
                }
                return;
            }
            GLOBAL_LOG_TRACE() << "after sigqueue";
        }
        for (thread & t : threads) {
            t.join();
        }
        GLOBAL_LOG_TRACE() << "~watch_descriptor";
    }
};

std::atomic <long> wd_refs_{ 0 };
std::atomic<watch_descriptor *> awd_{nullptr};

watch_descriptor* lock_wd() {
    for (;;) {
        long expected = wd_refs_.load();
        if (!expected) return nullptr;
        if (wd_refs_.compare_exchange_strong(expected, expected + 1)) break;
    }
    return awd_.load();
}

void unlock_wd() {
    if (wd_refs_.fetch_sub(1) == 1) {
        watch_descriptor* exp = awd_.load();
        if (!exp) return;
        if (!awd_.compare_exchange_strong(exp, nullptr)) {
            return;
        }

        delete exp;
    }
}

void set_user_signal(int val)
{
    user_signal_ = val;
}

int get_user_signal()
{
    return user_signal_;
}

void set_interruption_handler(user_handler_type const& ih)
{
    interruption_handler_ = ih;
}

void invoke_handler(void* fptr)
{
    if (!fptr) return;
    uintptr_t fid = (uintptr_t)fptr;
    if (0 != (fid & 1)) { // odd => fid
        if (watch_descriptor* wd = lock_wd(); wd) {
            SCOPE_EXIT([] { unlock_wd(); });
            shared_ptr<handler_entry> he = wd->get_entry(fid);
            if (he) {
                lock_guard eguard(he->mtx);
                if (he->handler) he->handler();
                if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << "a handler " << fid << " has been invoked";
            } else {
                if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << "a handler " << fid << " is obsoleted";
            }
        }
    } else {
        (*reinterpret_cast<user_handler_type*>(fptr))();
    }
}

void watcher_proc(int num)
{
    if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << ("start signal watcher %1%"_fmt % num).str();

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
            if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_ERROR() << "watcher %1%, sigwaitinfo error: %2%"_fmt % num % strerror(err);
            continue;
        }
        if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << ("watcher %1% received a signal %2%, code %3%"_fmt % num % sig % siginfo.si_code).str();
        
        try {
            if (sig == user_signal_) {
                int code = siginfo.si_code;
                if (SI_USER == code) break; // kill

                if (SI_QUEUE == code || SI_ASYNCIO == code || SI_TIMER == code) {
                    void * fptr = siginfo.si_value.sival_ptr;
                    if (!fptr) break; // stop_watchers
                    invoke_handler(fptr);
                } else {
                    if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << ("user signal %1% is ignored"_fmt % sig).str();
                }
            } else if (sig == SIGINT) {
                if (interruption_handler_) {
                    interruption_handler_();
                }
            } else {
                if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << ("signal %1% is ignored"_fmt % sig).str();
            }
        } catch (std::exception const& e) {
            GLOBAL_LOG_ERROR() << boost::diagnostic_information(e);
        } catch (...) {
            GLOBAL_LOG_ERROR() << boost::current_exception_diagnostic_information();
        }
    }

    if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << ("watcher %1% stopped"_fmt % num).str();
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
    auto wdptr = std::make_unique<watch_descriptor>();
    wdptr->threads.reserve(count);
    watch_descriptor* exp = nullptr;
    if (!awd_.compare_exchange_strong(exp, wdptr.get())) {
        return;
    }
    wd_refs_.store(1);

#ifndef __ANDROID__
    sigset_t new_mask;
    sigfillset(&new_mask);
    sigdelset(&new_mask, SIGSEGV);
    sigdelset(&new_mask, SIGBUS);
    pthread_sigmask(SIG_BLOCK, &new_mask, nullptr);

    watch_descriptor* wd = wdptr.release(); // unpin
    lock_wd();
    SCOPE_EXIT([] { unlock_wd(); });
    for (int c = 0; c < count; ++c) {
        wd->threads.emplace_back([c] { watcher_proc(c); });
    }
    if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << "signal watchers have been started, user signal: " << user_signal_;
#else
    wdptr.release(); // unpin
    if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << "signal 'watcher' has been started";
#endif
}

void stop_watchers() noexcept
{
    unlock_wd();
}

intptr_t register_handler(shared_ptr<handler_entry> e)
{
    if (watch_descriptor * wd = lock_wd(); wd) {
        SCOPE_EXIT([] { unlock_wd(); });
        intptr_t id = wd->new_entry(std::move(e));
        if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << "register handler entry: " << id;
        return id;
    }
    
    return 0;
}

void unregister_handler(intptr_t id)
{
    if (watch_descriptor* wd = lock_wd(); wd) {
        SCOPE_EXIT([] { unlock_wd(); });
        wd->delete_entry(id);
        if constexpr (SIGNAL_DEBUG) GLOBAL_LOG_TRACE() << "deleted handler entry: " << id;
    }
}

}
