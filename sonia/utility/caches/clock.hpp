//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_CACHES_CLOCK_HPP
#define SONIA_CACHES_CLOCK_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

// based on 
// CLOCK algorithm described in "A paging experiment with the multics system"
// https://www.researchgate.net/publication/235109943_A_paging_experiment_with_the_multics_system

#include <atomic>

#include <boost/call_traits.hpp>

#include "sonia/cstdint.hpp"

#include "utiltiy.hpp"

namespace sonia { namespace cashe {

template <typename KeyT, typename ValueT>
class clock {
    template <class> friend class handle;
    typedef KeyT key_type;
    typedef typename boost::call_traits<KeyT>::param_type key_param_type;

public:
    typedef handle<clock> handle_type;

    struct item {
        typedef boost::intrusive::unordered_set_member_hook<> key_hook_type;

        KeyT key;
        ValueT value;
        key_hook_type key_hook;
        std::atomic<uint16_t> refs;
        std::atomic<uint16_t> k;
    };

    template <typename ... ArgsT>
    handle_type get(key_param_type k, ArgsT&& ... args) {
    
    }

private:
    void release(item & i);


};


template <typename KeyT, typename ValueT>
void clock<KeyT, ValueT>::release(item & i) {
    if (1 == --i.refs) {
    
    }
}

}}

#endif // SONIA_CACHES_CLOCK_HPP
