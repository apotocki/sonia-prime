//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

class identifier_redefinition_error : public general_error
{
    variant<qname_identifier, identifier> name_;
    lex::resource_location location_;
    lex::resource_location reflocation_;

public:
    template <typename SomethingT>
    identifier_redefinition_error(annotated<SomethingT> n, lex::resource_location refloc)
        : name_{ std::move(n.value) }, location_{ std::move(n.location) }, reflocation_{ std::move(refloc) }
    {
    }

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return location_; }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override { return "identifier redefinition"sv; }
    lex::resource_location const* ref_location() const noexcept override { return &reflocation_; }
};

}
