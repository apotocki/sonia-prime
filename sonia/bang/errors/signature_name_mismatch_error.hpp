//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/errors.hpp"
#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class signature_name_mismatch_error : public basic_general_error
{
    qname_identifier actual_;
    qname_identifier expected_;

public:
    signature_name_mismatch_error(lex::resource_location loc, qname_identifier actual, qname_identifier expected, lex::resource_location refloc = {})
        : basic_general_error{ std::move(loc), ""sv, nullptr, std::move(refloc) }
        , actual_{ actual }
        , expected_{ expected }
    {}

    string_t description(unit const&) const noexcept override;
};

}
