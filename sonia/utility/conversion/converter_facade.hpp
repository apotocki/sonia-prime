/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <utility>

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>

#include <boost/mpl/inherit.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/range/value_type.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "sonia/type_traits.hpp"

#include "error_handling.hpp"

namespace sonia::conversion {
    
namespace detail {

// (e, (e, (e,...)))
template <typename RLT, size_t N> struct get_rl_element;

template <typename T0, typename T1> struct get_rl_element<std::pair<T0, T1>, 0> {
    typedef T0 type;
    inline static T0 & get(std::pair<T0, T1> & list) { return list.first; }
    inline static T0 const& get(std::pair<T0, T1> const& list) { return list.first; }
};

template <typename T> struct get_rl_element<T, 0> {
    typedef T type;
    inline static T & get(T & list) { return list; }
    inline static T const& get(T const& list) { return list; }
};

template <typename T0, typename T1, size_t N> struct get_rl_element<std::pair<T0, T1>, N> {
    typedef typename get_rl_element<T1, N-1>::type type;
    inline static typename get_rl_element<T1, N-1>::type & get(std::pair<T0, T1> & list) { return get_rl_element<T1, N-1>::get(list.second); }
    inline static typename get_rl_element<T1, N-1>::type const& get(std::pair<T0, T1> const& list) { return get_rl_element<T1, N-1>::get(list.second); }
};

template <class ElemT, typename CT>
class default_provider
{
public:
    typedef ElemT source_type;

    struct gstate_t {
        typedef null_t type;
    };
    
    struct main_pstate_t : CT::pstate_t{};

    template <typename CVTArgT>
    inline static void gclear(CVTArgT const&, null_t&) {}

    inline static bool equal_gstates(null_t const&, null_t const&) { return true; }

    template <typename IteratorT>
    inline static bool get(std::pair<IteratorT, IteratorT> & in, null_t&, ElemT * result)
    {
        if (in.first != in.second) {
            new(result) ElemT(*in.first); ++in.first;
            return true;
        }
        return false; // eof
    }

    template <typename ForwardReadableIteratorT, typename ForwardWritableIteratorT>
    inline static ForwardWritableIteratorT get_range(ForwardReadableIteratorT& in, null_t&, ForwardWritableIteratorT beginDest, ForwardWritableIteratorT endDest)
    {
        auto & from = in.first;
        while (from != in.second && beginDest != endDest)
        {
            *beginDest = *from;
            ++beginDest; ++from;
        }
        return beginDest;
    }

    template <int N, typename CVTArgT, typename StateT>
    inline static void pclear(CVTArgT const& cp, StateT&) {
        BOOST_STATIC_ASSERT (N == -1);
    }

    template <typename StateT>
    inline static bool equal_pstates(StateT const& s1, StateT const& s2) {
        return CT::back_equal_pstates(s1, s2);
    }

    template <class ForwardWritableIteratorT, class StateT>
    inline static void put(ElemT c, ForwardWritableIteratorT& out, StateT& s)
    {
        CT::back_put(c, out, s);
    }

    template <class ForwardWritableIteratorT, typename StateT>
    inline static void unshift(ForwardWritableIteratorT& out, StateT & s)
    {
        CT::back_unshift(out, s);
    }
};

template <class ElemT>
class default_customer
{
public:
    typedef ElemT destination_type;

    struct pstate_t {
        typedef null_t type;
    };

    template <typename CVTArgT>
    inline static void back_pclear(CVTArgT const&, null_t&) {}

    inline static bool back_equal_pstates(null_t const&, null_t const&) {
        return true;
    }

    template <class ForwardWritableIteratorT>
    inline static void back_put(ElemT c, ForwardWritableIteratorT& out, null_t&)
    {
        *out = c; ++out;
    }

    template <class ForwardReadableRange, class ForwardWritableIteratorT>
    inline static void back_put_range(ForwardReadableRange const& rng, ForwardWritableIteratorT& out, null_t&)
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                typename boost::range_value<ForwardReadableRange>::type, ElemT>::value
        ));
        std::copy(boost::begin(rng), boost::end(rng), out);
    }

    template <class ForwardWritableIteratorT>
    inline static void back_unshift(ForwardWritableIteratorT&, null_t&)
    {
        // do nothing
    }
};

} // namespace sonia::conversion::detail

template <
      class DerivedT
    , class CustomerT
    , class ProviderT
    , class SrcT
    , class DstT
    , class PStateT
    , class GStateT
>
class converter_facade : public boost::mpl::if_<
      boost::is_same<void, ProviderT>
    , detail::default_provider<SrcT, DerivedT>
    , ProviderT>::type
{
#if defined(BOOST_MSVC) | defined(BOOST_INTEL_CXX_VERSION)
    friend identity<DerivedT>::type;
#else
    friend DerivedT;
#endif

    typedef typename boost::mpl::if_<
          boost::is_same<void, ProviderT>
        , detail::default_provider<SrcT, DerivedT>
        , ProviderT
    >::type provider_t;

    typedef typename boost::mpl::if_<
          boost::is_same<void, CustomerT>
        , detail::default_customer<DstT>
        , CustomerT
    >::type customer_t;

    typedef SrcT local_source_type;

public:
    typedef converter_facade facade_type;

    struct pstate_t {
        typedef std::pair<PStateT, typename customer_t::pstate_t::type> type;
    };
    
    struct gstate_t {
        typedef std::pair<GStateT, typename provider_t::gstate_t::type> type;
    };

    typedef DstT destination_type;

    template <typename StateT>
    inline static bool do_equal_states(StateT const& s1, StateT const& s2) { return s1 == s2; }

    template <typename T0, typename T1>
    inline static T1 const& do_get_provider_tag(std::pair<T0, T1> const& cp) {
        return cp.second;
    }

    template <typename T>
    inline static T const& do_get_provider_tag(T const& t) {
        return t;
    }

    template <typename T0, typename T1, class StateT>
    inline static void gclear(std::pair<T0, T1> const& cp, StateT& st) {
        DerivedT::do_clear(pvalue(do_get_provider_tag(cp.first)), cvalue(cp.second), st.first);
        provider_t::gclear(cp.first, st.second);
    }

    template <class StateT>
    inline static bool equal_gstates(StateT const& s1, StateT const& s2) {
        return DerivedT::do_equal_states(s1.first, s2.first) &&
            provider_t::equal_gstates(s1.second, s2.second);
    }

    ////////////////////// 'PUT' PART
    template <int N, typename T0, typename T1, typename StateT>
    inline static void pclear(std::pair<T0, T1> const& cp, StateT& st)
    {
        DerivedT::do_clear(pvalue(do_get_provider_tag(cp.first)), cvalue(cp.second), detail::get_rl_element<StateT, N>::get(st));
        provider_t::template pclear<N-1>(cp.first, st);
    }

    template <class StateT>
    inline static bool back_equal_pstates(StateT const& s1, StateT const& s2) {
        return DerivedT::do_equal_states(s1.first, s2.first) &&
            customer_t::back_equal_pstates(s1.second, s2.second);
    }

    template <class ForwardReadableRange, class ForwardWritableIteratorT, typename StateT>
    inline static void back_put_range(ForwardReadableRange const& rng, ForwardWritableIteratorT& out, StateT& s)
    {
        DerivedT::do_put_range(rng, out, s);
    }

    template <class ForwardWritableIteratorT, typename StateT>
    inline static void back_put(local_source_type c, ForwardWritableIteratorT& out, StateT& s)
    {
        DerivedT::do_put(c, out, s);
    }

    template <class ForwardWritableIteratorT, typename StateT>
    inline static void back_unshift(ForwardWritableIteratorT& out, StateT & st)
    {
        DerivedT::do_unshift(out, st);
        customer_t::back_unshift(out, st.second);
    }

    //template <class TH, class ForwardWritableIteratorT, typename S0, typename S1>
    //inline static void back_unshift(ForwardWritableIteratorT& out, std::pair<S0, S1> & st)
    //{
    //    DerivedT::do_unshift(out, st);
    //    customer_t::back_unshift(out, st.second);
    //}

    template <class ForwardWritableIteratorT, typename StateT>
    inline static void customer_put(destination_type c, ForwardWritableIteratorT& out, StateT& st)
    {
        customer_t::back_put(c, out, st.second);
    }

    template <class ForwardReadableRange, class ForwardWritableIteratorT, typename StateT>
    inline static void customer_put_range(ForwardReadableRange const& rng, ForwardWritableIteratorT& out, StateT& st)
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
            typename boost::range_value<ForwardReadableRange>::type, destination_type>::value
        ));
        customer_t::back_put_range(rng, out, st.second);
    }

    ////////////////////// 'GET' PART
    template <typename ForwardReadableIteratorT>
    inline static bool get(ForwardReadableIteratorT& in, typename gstate_t::type& st, destination_type * result)
    {
        return DerivedT::do_get(in, st, result);
    }

    template <typename ForwardReadableIteratorT>
    inline static bool provider_get(ForwardReadableIteratorT& in, typename gstate_t::type& st, local_source_type * result)
    {
        return provider_t::get(in, st.second, result);
    }

    template <typename ForwardReadableIteratorT, typename ForwardWritableIteratorT>
    inline static ForwardWritableIteratorT get_range(ForwardReadableIteratorT& in, typename gstate_t::type& st, ForwardWritableIteratorT beginDest, ForwardWritableIteratorT endDest)
    {
        return DerivedT::do_get_range(in, st, beginDest, endDest);
    }

    template <class ForwardReadableIteratorT, typename ForwardWritableIteratorT>
    inline static ForwardWritableIteratorT provider_get_range(ForwardReadableIteratorT& in, typename gstate_t::type& st, ForwardWritableIteratorT beginDest, ForwardWritableIteratorT endDest)
    {
        return provider_t::get_range(in, st.second, beginDest, endDest);
    }

    // default implementation
    template <class ForwardReadableIteratorT>
    inline static destination_type * do_get_range(ForwardReadableIteratorT& in, typename gstate_t::type& st, destination_type * beginDest, destination_type * endDest)
    {
        for (; beginDest != endDest; ++beginDest)
        {
            if (!DerivedT::do_get(in, st, beginDest)) break;
        }
        return beginDest;
    }

protected:
    BOOST_FORCEINLINE static GStateT& gstate(typename gstate_t::type& st) { return st.first; }
    
    template <typename StateT>
    BOOST_FORCEINLINE static PStateT& pstate(StateT& st) { return st.first; }
};

}
