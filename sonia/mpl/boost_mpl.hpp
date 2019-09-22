//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_BOOST_HPP
#define SONIA_MPL_BOOST_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/mpl/at.hpp>

namespace sonia::mpl {

template <> struct at_impl<void>
{
    template <class SeqT, class IdxT> struct apply : boost::mpl::at<SeqT, IdxT> {};
};

}

#endif // SONIA_MPL_BOOST_HPP
