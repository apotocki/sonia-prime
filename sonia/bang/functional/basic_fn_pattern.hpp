//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/small_vector.hpp"
#include "sonia/bang/entities/functional.hpp"

#include "parameter_matcher.hpp"

namespace sonia::lang::bang {

class internal_function_entity;

class basic_fn_pattern : public functional::pattern
{
protected:
    struct parameter_descriptor
    {
        annotated_identifier ename;
        small_vector<annotated_identifier, 2> inames;
        variant<constraint_expression_t, pattern_t> constraint;
        optional<syntax_expression_t> default_value;
    };

    // in the order of declaration (fn_pure)
    small_vector<parameter_descriptor, 8> parameters_;

    boost::container::small_flat_set<named_parameter_matcher, 8, named_parameter_matcher_less> named_matchers_;
    small_vector<parameter_matcher, 8> matchers_;

    variant<nullptr_t, syntax_expression_t, pattern_t> result_;

public:
    basic_fn_pattern() = default;

    //error_storage init(fn_compiler_context&, parameter_list_t const&);
    
    error_storage init(fn_compiler_context&, fn_pure_t const&);


    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, prepared_call const&, expected_result_t const&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override;

protected:
    std::pair<syntax_expression_result_t, size_t> apply_arguments(fn_compiler_context&, semantic::expression_list_t&, functional_match_descriptor&) const;
};

}
