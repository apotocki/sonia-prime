//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SAL_TYPES_HPP
#define SONIA_SAL_TYPES_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

namespace sonia { namespace sal {

#ifdef BOOST_WINDOWS
typedef HANDLE file_handle_type;
#else
typedef int file_handle_type;
#endif

}}

#endif // SONIA_SAL_HPP
