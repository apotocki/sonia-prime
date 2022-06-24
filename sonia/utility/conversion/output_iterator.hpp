/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <iterator>
#include <utility>

namespace sonia::conversion {
    
template <
      typename ConverterT
    , typename OutputIteratorT
>
class convert_output_iterator
{
public:
    typedef std::output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    static const size_t cvt_pipe_length = ConverterT::length;
    typedef typename ConverterT::template apply<void>::type converter_t;
    typedef typename converter_t::main_pstate_t::type state_type;

    template <typename CVTExprOrObjectT>
    convert_output_iterator(CVTExprOrObjectT const& expr, state_type & st, OutputIteratorT extern_it)
        : proxy_(expr, st, std::move(extern_it))
    { }

    struct output_proxy {
        template <typename CVTExprOrObjectT>
        output_proxy(CVTExprOrObjectT const& expr, state_type & st, OutputIteratorT const& extern_it) 
            : state_(boost::addressof(st))
            , output_(extern_it)
        {
            converter_t::template pclear<cvt_pipe_length-1>(expr.get_init_tag(), *state_);
        }

        template <class T>
        output_proxy& operator=(T const& value) {
            converter_t::put(value, output_, *state_);
            return *this; 
        }

        void operator= (output_proxy const& rhs) {
            output_ = rhs.output_;
            state_ = rhs.state_;
        }

        inline OutputIteratorT unshift() {
            converter_t::unshift(output_, *state_);
            return std::move(output_);
        }

    private:
        state_type * state_;
        OutputIteratorT output_;
    };

    inline auto unshift() { return proxy_.unshift(); }

    inline output_proxy& operator*() { return proxy_; }
    inline convert_output_iterator& operator++() { return *this; } 
    inline convert_output_iterator& operator++(int) { return *this; }

private:
    output_proxy proxy_;
};

}
