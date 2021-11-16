/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <limits.h>
#include <utility>

#include <boost/assert.hpp>

namespace sonia::conversion {

template <typename ErrorHandlerT>
struct cpstate_wc : private ErrorHandlerT
{
    typedef std::codecvt<wchar_t, char, std::mbstate_t> facet_t;

    static const size_t wcharcache_size = 2;
    std::mbstate_t facetstate_;
    wchar_t wcharcache_[wcharcache_size];
    unsigned int wcharcache_length:4;
    std::locale locale_;
    facet_t const* facet_;

    template <typename ArgT>
    void clear(ArgT const& arg)
    {
        locale_ = std::locale(arg.localename_);
        facet_ = boost::addressof(std::use_facet<facet_t>(locale_));

        static_cast<ErrorHandlerT&>(*this) = arg.errh_;
        facetstate_ = std::mbstate_t(); wcharcache_length = 0;
    }

    inline bool operator==(cpstate_wc const& rhs) const
    {
        return wcharcache_length == rhs.wcharcache_length &&
            std::equal(wcharcache_
                , wcharcache_ + wcharcache_length
                , rhs.wcharcache_);
    }

    inline void push_wchar(wchar_t w)
    {
        BOOST_ASSERT(wcharcache_length < sizeof(wcharcache_)/sizeof(wchar_t));
        wcharcache_[wcharcache_length++] = w;
    }

    inline void shift_wcharcache(wchar_t const* from)
    {
        std::copy<const wchar_t*>(from, wcharcache_ + wcharcache_length, wcharcache_);
        wcharcache_length -= static_cast<unsigned int>(from - wcharcache_);
    }

    template <typename ArgT>
    inline void error(ArgT && c, const char *errmsg) {
        ErrorHandlerT::operator()(std::forward<ArgT>(c), *this, errmsg);
    }
};

template <typename ErrorHandlerT>
struct cpstate : private ErrorHandlerT
{
    typedef std::codecvt<wchar_t, char, std::mbstate_t> facet_t;

    static const size_t charcache_size = MB_LEN_MAX;
    static const size_t wcharcache_size = 2;

    std::mbstate_t facetstate_;
    char charcache_[charcache_size];
    wchar_t wcharcache_[wcharcache_size];

    unsigned int charcache_length:8;
    unsigned int charcache_start:8;
    unsigned int wcharcache_length:8;
    unsigned int wcharcache_start:8;

    std::locale locale_;
    facet_t const* facet_;

    template <typename ArgT>
    void clear(ArgT const& arg)
    {
        locale_ = std::locale(arg.localename_);
        facet_ = boost::addressof(std::use_facet<facet_t>(locale_));

        static_cast<ErrorHandlerT&>(*this) = arg.errh_;
        facetstate_ = std::mbstate_t(); charcache_length = wcharcache_length = wcharcache_start  = charcache_start = 0;
    }

    inline bool operator==(cpstate const& rhs) const
    {
        return charcache_length == rhs.charcache_length && wcharcache_length == rhs.wcharcache_length &&
            std::equal(charcache_ + charcache_start
                , charcache_ + charcache_start + charcache_length
                , rhs.charcache_ + rhs.charcache_start) &&
            std::equal(wcharcache_ + wcharcache_start
                , wcharcache_ + wcharcache_start + wcharcache_length
                , rhs.wcharcache_ + rhs.wcharcache_start);
    }

    inline void push_char(char c)
    {
        BOOST_ASSERT(charcache_length < charcache_size);
        charcache_[charcache_start + charcache_length++] = c;
    }

    inline char pop_char()
    {
        BOOST_ASSERT(charcache_length < sizeof(charcache_));
        char result = charcache_[charcache_start++]; --charcache_length;
        return result;
    }

    inline void shift_charcache(char const* from)
    {
        std::copy<const char*>(from, charcache_ + charcache_length, charcache_);
        charcache_length -= static_cast<unsigned int>(from - charcache_);
    }

    inline void push_wchar(wchar_t wc)
    {
        BOOST_ASSERT(wcharcache_length < sizeof(wcharcache_)/sizeof(wchar_t));
        wcharcache_[wcharcache_start + wcharcache_length++] = wc;
    }

    inline wchar_t pop_wchar()
    {
        BOOST_ASSERT(wcharcache_length < sizeof(wcharcache_));
        wchar_t result = wcharcache_[wcharcache_start++]; --wcharcache_length;
        return result;
    }

    inline void shift_wcharcache(wchar_t const* from)
    {
        std::copy<const wchar_t*>(from, wcharcache_ + wcharcache_length, wcharcache_);
        wcharcache_length -= static_cast<unsigned int>(from - wcharcache_);
    }

    template <typename ArgT>
    inline void error(ArgT && c, const char *errmsg) {
        ErrorHandlerT::operator()(std::forward<ArgT>(c), *this, errmsg);
    }
};

}
