//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/errors.hpp"
#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class cast_error : public basic_general_error
{
    entity_identifier src_;
    entity_identifier dest_;

public:
    cast_error(lex::resource_location loc, entity_identifier src, entity_identifier dest, string_t descr = ""sv, lex::resource_location refloc = {})
        : basic_general_error{ std::move(loc), std::move(descr), nullptr, std::move(refloc) }
        , src_{ src }
        , dest_{ dest }
    {}

    string_t description(unit const&) const noexcept override;
};

}
