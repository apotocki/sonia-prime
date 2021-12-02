/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

//#include "converter.hpp"
#include "composite.hpp"
#include "input_iterator.hpp" 
#include "output_iterator.hpp" 

#include "sonia/iterator_traits.hpp"

//#include "boost/range/const_iterator.hpp"
//#include "boost/range/begin.hpp"
//#include "boost/range/end.hpp"

namespace sonia::conversion {

template <typename InputRangeT, typename OutputIteratorT, typename ConverterT>
void pull(InputRangeT const& rng, OutputIteratorT oit, ConverterT const& enc)
{
    using InputIteratorT = range_iterator_t<const InputRangeT>;
    using it_t = convert_input_iterator<ConverterT, InputIteratorT>;
    using state_type = typename it_t::state_type;

    state_type state;
    std::copy(it_t(enc, state, boost::begin(rng), boost::end(rng)), it_t(), oit);
}

template <typename InputIteratorT, typename OutputIteratorT, typename ConverterT>
void pull(InputIteratorT b, InputIteratorT e, OutputIteratorT oit, ConverterT const& enc)
{
    typedef convert_input_iterator<ConverterT, InputIteratorT> it_t;
    typedef typename it_t::state_type state_type;
    
    state_type state;
    std::copy(it_t(enc, state, std::move(b), std::move(e)), it_t(), oit);
}

template <typename InputRangeT, typename OutputIteratorT, typename ConverterT>
void push(InputRangeT const& rng, OutputIteratorT oit, ConverterT const& enc)
{
    typedef typename boost::range_const_iterator<InputRangeT>::type InputIteratorT;
    typedef convert_output_iterator<ConverterT, OutputIteratorT> it_t;
    typedef typename it_t::state_type state_type;

    state_type state;
    std::copy(boost::begin(rng), boost::end(rng), it_t(enc, state, oit)).unshift();
}

template <typename InputIteratorT, typename OutputIteratorT, typename ConverterT>
void push(InputIteratorT b, InputIteratorT e, OutputIteratorT oit, ConverterT const& enc)
{
    typedef convert_output_iterator<ConverterT, OutputIteratorT> it_t;
    typedef typename it_t::state_type state_type;

    state_type state;
    std::copy(b, e, it_t(enc, state, oit)).unshift();
}

}
