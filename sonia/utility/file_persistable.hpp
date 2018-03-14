//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_FILE_PERSISTABLE_HPP
#define SONIA_UTILITY_FILE_PERSISTABLE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iterator>

#include <boost/iterator/iterator_facade.hpp>

namespace sonia {

class persistable {

    iterator<array_ref<char>, std::forward_iterator_tag, >
};

}

#endif // SONIA_UTILITY_FILE_PERSISTABLE_HPP
