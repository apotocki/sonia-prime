//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SAL_HPP
#define SONIA_SAL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "string.hpp"
#include "concurrency.hpp"
#include "shared_ptr.hpp"
#include "sal_types.hpp"
#include "sonia/services/bundle.hpp"

// system abstract level functions

namespace sonia { namespace sal {

void set_thread_name(sonia::thread::id, string_view name);
void set_thread_name(sonia::thread::id, std::string const& name);

shared_ptr<sonia::services::bundle> load_bundle(string_view name);

// file routine
uint64_t file_size(file_handle_type h);
void delete_file(cstring_view path);

}}

#endif // SONIA_SAL_HPP
