//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

class function_call_match_error : public general_error
{
public:
    annotated_qname_identifier functional_;
    //function_signature const* signature_;
    error_storage reason_;

    function_call_match_error(annotated_qname_identifier f, /*function_signature const* signature,*/ error_storage reason)
        : functional_{ f }
        //, signature_{ signature }
        , reason_{ reason }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    location_t location() const noexcept override { return functional_.location; }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override;
};

}
