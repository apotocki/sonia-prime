//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/small_vector.hpp"
#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

class fn_compiler_context;

class prepared_call
{
public:
    mutable semantic::managed_expression_list expressions;
    lex::resource_location location;
    small_vector<named_expression_t, 8> args;
    

    inline prepared_call(unit& u, lex::resource_location loc) noexcept
        : expressions{ u }
        , location{ std::move(loc) }
    {}

    explicit prepared_call(pure_call_t const&);

    void splice_back(semantic::expression_list_t&) const noexcept;
    void splice_back(semantic::expression_list_t&, semantic::expression_span) const noexcept;

    error_storage prepare(fn_compiler_context&);

    struct session
    {
        session() = default;
        session(session const&) = delete;
        session(session&&) = default;
        session& operator=(session const&) = delete;
        session& operator=(session&&) = delete;
        ~session() noexcept = default;
    
        prepared_call const& call;

        inline explicit session(prepared_call const& c) noexcept
            : call{ c }
        {}

        std::expected<syntax_expression_result_reference_t, error_storage> use(fn_compiler_context&, size_t arg_index, annotated_entity_identifier const& exp);
    };

    session new_session() const;
};

}
