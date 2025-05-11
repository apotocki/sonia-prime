//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/small_vector.hpp"
#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

#include <boost/container/flat_map.hpp>

namespace sonia::lang::bang {

class fn_compiler_context;

class prepared_call
{
public:
    semantic::expression_list_t& expressions;
    lex::resource_location location;
    small_vector<named_expression_t, 8> args;
    
    // cache
    using cache_key_t = std::tuple<entity_identifier, bool>;
    struct argument_cache
    {
        syntax_expression_t expression;
        boost::container::small_flat_map<cache_key_t, std::expected<syntax_expression_result_t, error_storage>, 8> cache;

        inline explicit argument_cache(syntax_expression_t e) noexcept
            : expression{ std::move(e) }
        {}
    };

    mutable small_vector<std::tuple<identifier, argument_cache>, 8> named_argument_caches_;
    mutable small_vector<argument_cache, 8> position_argument_caches_;

    inline prepared_call(semantic::expression_list_t& ael, lex::resource_location loc) noexcept
        : expressions{ ael }
        , location{ std::move(loc) }
    {}

    prepared_call(pure_call_t const&, semantic::expression_list_t&);

    //void splice_back(semantic::expression_list_t&) const noexcept;
    //void splice_back(semantic::expression_list_t&, semantic::expression_span) const noexcept;

    error_storage prepare(fn_compiler_context&);

    struct session
    {
        session(session const&) = delete;
        session(session&&) = default;
        session& operator=(session const&) = delete;
        session& operator=(session&&) = delete;
        ~session() noexcept = default;
    
        fn_compiler_context& ctx;
        
        prepared_call const& call;
        small_vector<std::tuple<identifier, argument_cache*>, 8> unused_named_arguments_;
        small_vector<argument_cache*, 8> unused_position_arguments_;
        small_vector<semantic::expression_span, 4> void_spans; // void argument's expressions
        size_t unused_positioned_index_ = 0;
        size_t unused_positioned_skipped_ = 0;

        session(fn_compiler_context&, prepared_call const&);

        std::expected<syntax_expression_result_t, error_storage> use_next_positioned_argument(syntax_expression_t const** pe = nullptr);
        std::expected<syntax_expression_result_t, error_storage> use_next_positioned_argument(annotated_entity_identifier const& exp, bool const_exp, syntax_expression_t const** pe = nullptr);
        
        named_expression_t unused_argument();

    private:
        std::expected<syntax_expression_result_t, error_storage>
        do_resolve(argument_cache& arg_cache, annotated_entity_identifier const& exp, bool const_exp);
    };

    session new_session(fn_compiler_context&) const;

private:
    

};

}
