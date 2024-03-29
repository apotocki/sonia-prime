//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#ifndef SONIA_PRIME_STATIC
#   ifdef SONIA_PRIME_EXPORTS
#       define SONIA_PRIME_API BOOST_SYMBOL_EXPORT
#   else
#       define SONIA_PRIME_API BOOST_SYMBOL_IMPORT
#   endif
#else
#   define SONIA_PRIME_API
#endif
