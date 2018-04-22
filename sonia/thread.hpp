//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_THREAD_HPP
#define SONIA_THREAD_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/thread.hpp>

namespace sonia {

using boost::thread;

namespace this_thread = boost::this_thread;

}

#endif // SONIA_THREAD_HPP
