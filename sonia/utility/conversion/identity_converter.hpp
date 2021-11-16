/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "tags.hpp"
#include "converter_facade.hpp"
#include "concrete_converter.hpp" 

namespace sonia::conversion {

template <typename elemT>
struct identity_tag : cvt_tag
{
    typedef identity_tag customer_tag;
    typedef identity_tag provider_tag;
};

template <typename elemT>
inline identity_tag<elemT> identity()
{ return identity_tag<elemT>(); }

inline identity_tag<char> identity()
{ return identity_tag<char>(); }

template <typename ProviderT, typename CustomerT, typename elemT>
class identity_converter 
    : public converter_facade<
          identity_converter<ProviderT, CustomerT, elemT>
        , CustomerT
        , ProviderT
        , elemT
        , elemT
        , null_t
        , null_t>
{
protected:
    typedef identity_converter<ProviderT, CustomerT, elemT> this_type;
    typedef converter_facade<
          this_type
        , CustomerT
        , ProviderT
        , elemT
        , elemT
        , null_t
        , null_t> base_type;

#if defined(BOOST_MSVC) | defined(BOOST_INTEL_CXX_VERSION)
    friend boost::mpl::identity<base_type>::type;
#else
    friend class boost::mpl::identity<base_type>::type;
#endif

public:
    identity_converter() : base_type()
    {}

    template <typename provider_init_tag>
    identity_converter(provider_init_tag const& t, identity_tag<elemT> const&) : base_type(t)
    {}

    template <typename customer_init_tag>
    identity_converter(identity_tag<elemT> const&, customer_init_tag const& t) : base_type()
    {}

    identity_converter(identity_tag<elemT> const&, identity_tag<elemT> const&) : base_type()
    {}
private:
    inline void do_clear(null_t& st) const {}
    inline bool do_equal_states(null_t const& s1, null_t const& s2) const { return true; }


    template <typename ForwardWritableIterator, typename state_t>
    inline void do_put(elemT c, ForwardWritableIterator& out, state_t& s) const
    {
        base_type::customer_put(c, out, s);
    }

    template <typename ForwardWritableIterator, typename state_t>
    inline void do_unshift(ForwardWritableIterator& out, state_t & s) const
    {
        // do nothing    
    }

    template <typename ForwardReadableIterator, typename state_t>
    optional<elemT> do_get(ForwardReadableIterator& in, state_t& s) const
    {
        return base_type::provider_get(in, s);
    }

    template <class ForwardReadableIterator, typename ForwardWritableIterator, typename state_t>
    inline ForwardWritableIterator do_get_range(ForwardReadableIterator& in, state_t& st, ForwardWritableIterator beginDest, ForwardWritableIterator endDest) const
    {
        return base_type::provider_get_range(in, st, beginDest, endDest);
    }
};

namespace detail {

template <typename any_tagT, typename T>
struct concrete_converter_impl<any_tagT, identity_tag<T> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef identity_converter<ProviderT, CustomerT, T> type;
    };
};

template <typename any_tagT, typename T>
struct concrete_converter_impl<identity_tag<T>, any_tagT>
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef identity_converter<ProviderT, CustomerT, T> type;
    };
}; 

template <typename T>
struct concrete_converter_impl<identity_tag<T>, identity_tag<T> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef identity_converter<ProviderT, CustomerT, T> type;
    };
};

}}

}
