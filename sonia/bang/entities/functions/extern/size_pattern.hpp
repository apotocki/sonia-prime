//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/generic_pattern_base.hpp"

namespace sonia::lang::bang {

class size_pattern : public generic_pattern_base<size_pattern>
{
    friend class generic_pattern_base<size_pattern>;

public:
    size_pattern() = default;

    std::expected<syntax_expression_result_t, error_storage> apply(fn_compiler_context&, functional_match_descriptor&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override { return s << "size(const signature)->integer"; }

protected:
    template <typename ArgApplicationT>
    error_storage accept_argument(std::nullptr_t, functional_match_descriptor_ptr&, arg_context_type&, ArgApplicationT&) const;
};

}
