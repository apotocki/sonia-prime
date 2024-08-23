//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/lang/utility.hpp"
#include "sonia/utility/lang/qname.hpp"

namespace sonia::lang::bang {

struct lex_tag;
struct enity_tag;

using identifier = sonia::lang::identifier<uint32_t, lex_tag>;
using qname = sonia::lang::qname<identifier>;
using qname_view = sonia::lang::qname_view<identifier>;
using entity_identifier = sonia::lang::identifier<uint32_t, enity_tag>;

}
