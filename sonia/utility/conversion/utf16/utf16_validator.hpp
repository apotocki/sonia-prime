/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <utility>
#include <sstream>

#include "../encoder_facade.hpp"

#include "state.hpp"
#include "constants.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT, typename OutT, typename PstateT, typename GstateT>
class utf16_validator
    : public encoder_facade<
          utf16_validator<ProviderT, CustomerT, OutT, PstateT, GstateT>
        , CustomerT
        , ProviderT
        , char16_t
        , OutT
        , std::pair<char16_t, PstateT>
        , std::pair<char16_t, GstateT>
    >
{
protected:
    typedef encoder_facade<
          utf16_validator
        , CustomerT
        , ProviderT
        , char16_t
        , OutT
        , std::pair<char16_t, PstateT>
        , std::pair<char16_t, GstateT> > facade_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend facade_type;
#else
    friend class facade_type;
#endif 

public:
    template <typename provider_init_tag>
    utf16_validator(provider_init_tag const& t, ErrorHandlerT const& errh) : facade_type(t), errh_(errh)
    {}

private:
    typedef std::pair<char16_t, PstateT> pstate_t_;
    typedef std::pair<char16_t, GstateT> gstate_t_;

    template <typename state_t>
    inline void do_clear(state_t& st) const { st.first() = 0; derived().do_clear_impl(st.second()); }

    template <typename ForwardWritableIterator, typename state_t>
    static void do_put(char16_t val, ForwardWritableIterator& out, state_t& s)
    {
        typename pstate_t_ & st = facade_type::pstate(s);
        if (!st.first())
        {
            if (val < UTF16_SUR_HIGH_START || val > UTF16_SUR_LOW_END) {
                derived().do_put(val, out, s);
            } else if (val <= UTF16_SUR_HIGH_END) { // found high surrogate
                st.first() = val;
            } else {
                std::stringstream serr;
                serr << "Wrong utf16 character '" << std::hex << val << "'(low surrogate is not expected)";
                errh_(val, st.first(), serr.str().c_str());
            }
        } else {
            if (val >= UTF16_SUR_LOW_START && val <= UTF16_SUR_LOW_END) {
                derived().do_put(st.first(), out, s);
                derived().do_put(val, out, s);
                st.first() = 0;
            } else {
                std::stringstream serr;
                serr << "Wrong utf16 character '" << std::hex << val << "'(low surrogate is expected)";
                errh_(val, st.first(), serr.str().c_str());
                st.first() = 0;
            }
        }
    }

    template <typename ForwardWritableIterator, typename state_t>
    inline static void do_unshift(ForwardWritableIterator& out, state_t & s)
    {
        // do nothing
    }

protected:
    template <typename ForwardReadableIterator, typename state_t>
    static optional<char16_t> do_get_impl(ForwardReadableIterator& in, state_t& s)
    {
        auto & st = facade_type::gstate(s);
        for (;;) {
            if (st.first()) {
                char16_t result = st.first(); st.first() = 0;
                return result;
            }
            optional<char16_t> ov = facade_type::provider_get(in, s);
            if (!ov) return optional<char16_t>(); // eof

            char16_t result = ov.get();
            if (result < UTF16_SUR_HIGH_START || result > UTF16_SUR_LOW_END) {
                return result;
            } else if (result <= UTF16_SUR_HIGH_END) { // found high surrogate
                ov = facade_type::provider_get(in, s);
                if (!ov) {
                    errh_(result, null_t(), "Unexpected end of UTF16 data.");
                    return optional<char16_t>();
                }
                char16_t trail = ov.get();
                if (trail >= UTF16_SUR_LOW_START || trail <= UTF16_SUR_LOW_END) {
                    st.first() = trail;
                    return result;
                } else {
                    errh_(trail, null_t(), "Low surrogate is expected.");
                }
            } else {
                errh_(result, null_t(), "Low surrogate is not expected.");
            }
        }
    }
};

}
