//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_map.hpp>

#include "sonia/span.hpp"
#include "sonia/string.hpp"
#include "sonia/singleton.hpp"
#include "sonia/function.hpp"
#include "sonia/utility/invokation/invokation.hpp"

namespace sonia::invokation {

class value_encoder : public singleton
{
public:
    void encode(blob_result const& value, string_view type, string_view endianness, function<void(span<const uint8_t>)> const& writer) const;

    value_encoder();

private:
    using value_handler = void(*)(blob_result const&, string_view, function<void(span<const uint8_t>)> const&);
    boost::unordered_map<std::string, value_handler, hasher, string_equal_to> value_handlers_;
};
}
