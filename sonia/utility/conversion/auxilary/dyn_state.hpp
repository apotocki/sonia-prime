/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <new>
#include <boost/scoped_array.hpp>

namespace sonia::conversion::auxilary {

class dyn_state
{
public:
    dyn_state(dyn_state const&) = delete;

    template <typename typeT>
    inline void create() {
        store_.reset(new char[sizeof(typeT)]);
        ::new(store_.get()) typeT;
        sz_ = sizeof(typeT);
    }

    template <typename typeT>
    inline typeT & get() {
        return static_cast<typeT&>(*reinterpret_cast<typeT*>(store_.get()));
    }

    template <typename typeT>
    inline typeT const & get() const {
        return static_cast<typeT const&>(*reinterpret_cast<typeT const*>(store_.get()));
    }

    inline bool empty() const { return sz_ == 0; }

    dyn_state() : sz_(0) {}

    dyn_state(dyn_state const& rhs) : sz_(rhs.sz_)
    {
        if (sz_) {
            store_.reset(new char[sz_]);
            std::copy(rhs.store_.get(), rhs.store_.get() + sz_, store_.get());
        }
    }

    void operator= (dyn_state const& rhs) {
        sz_ = rhs.sz_;
        store_.reset(new char[sz_]);
        std::copy(rhs.store_.get(), rhs.store_.get() + sz_, store_.get());
    }

// members
private:
    size_t sz_;
    boost::scoped_array<char> store_;
};

} // namespace sonia::conversion::auxilary
