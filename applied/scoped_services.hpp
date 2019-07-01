//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef APPLIED_SCOPED_SERVICES_HPP
#define APPLIED_SCOPED_SERVICES_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/concurrency.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/services.hpp"

class scoped_services
{
public:
    scoped_services(scoped_services const&) = delete;
    scoped_services& operator=(scoped_services const&) = delete;

    explicit scoped_services(int argc = 0, char const* argv[] = nullptr, std::istream * cfgstream = nullptr)
    {
        pre_initialize(argc, argv);
        sonia::services::initialize(argc, argv, cfgstream);
    }

    explicit scoped_services(std::istream & cfgstream)
        : scoped_services{0, nullptr, &cfgstream}
    {}

    ~scoped_services()
    {
        sonia::services::shutdown();
    }

    void run()
    {
        run_ = true;
        sonia::unique_lock lock(mtx_);
        while (run_) {
            var_.wait(lock);
        }
    }

    void stop()
    {
        sonia::unique_lock lock(mtx_);
        run_ = false;
        var_.notify_one();
    }

    void pre_initialize(int& argc, char const**& argv);

private:
    sonia::mutex mtx_;
    sonia::condition_variable var_;
    bool run_;
};

#endif // APPLIED_SCOPED_SERVICES_HPP
