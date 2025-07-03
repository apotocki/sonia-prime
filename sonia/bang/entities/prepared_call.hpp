//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/small_vector.hpp"
#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

#include <boost/container/flat_map.hpp>

#include "functional.hpp"

namespace sonia::lang::bang {

class fn_compiler_context;

class prepared_call
{
public:
    fn_compiler_context& caller_ctx;
    functional const* pfnl;
    semantic::expression_list_t& expressions;
    lex::resource_location location;
    small_vector<named_expression_t, 8> args;
    functional_binding_set bound_temporaries;
    small_vector<std::tuple<identifier, local_variable*, semantic::expression_span>, 4> temporaries;

    // cache
    using cache_key_t = std::tuple<entity_identifier, value_modifier_t>;
    struct argument_cache
    {
        syntax_expression_t expression;
        boost::container::small_flat_map<cache_key_t, std::expected<std::pair<syntax_expression_result_t, bool>, error_storage>, 8> cache;

        inline explicit argument_cache(syntax_expression_t e) noexcept
            : expression{ std::move(e) }
        {}
    };

    mutable small_vector<std::tuple<identifier, argument_cache>, 8> argument_caches_;
    uint64_t named_map_, positioned_map_; // bitmasks of named and positional arguments

    //mutable small_vector<std::tuple<identifier, argument_cache>, 8> named_argument_caches_;
    //mutable small_vector<argument_cache, 8> position_argument_caches_;

    prepared_call(fn_compiler_context&, functional const*, semantic::expression_list_t& ael, lex::resource_location loc) noexcept;
    prepared_call(fn_compiler_context&, functional const*, pure_call_t const&, semantic::expression_list_t&);
    prepared_call(prepared_call const&) = delete;
    ~prepared_call();

    prepared_call& operator=(prepared_call const&) = delete;

    error_storage prepare();

    using argument_descriptor_t = std::pair<syntax_expression_t const*, size_t>;
    using next_argument_descriptor_t = std::tuple<identifier, syntax_expression_t const*, size_t>;

    struct session
    {
        session(session const&) = delete;
        session(session&&) = default;
        session& operator=(session const&) = delete;
        session& operator=(session&&) = delete;
        ~session() noexcept = default;
    
        fn_compiler_context& ctx;
        
        prepared_call const& call;
        uint64_t named_usage_map_, positioned_usage_map_; // bitmasks of unused named and positional arguments
        //small_vector<std::tuple<identifier, argument_cache*>, 8> unused_named_arguments_;
        //small_vector<argument_cache*, 8> unused_position_arguments_;
        small_vector<semantic::expression_span, 4> void_spans; // void argument's expressions
        //size_t unused_positioned_index_ = 0;
        //size_t unused_positioned_skipped_ = 0;

        session(fn_compiler_context&, prepared_call const&);

        bool has_more_positioned_arguments() const noexcept;
        std::expected<std::pair<syntax_expression_result_t, bool>, error_storage> use_next_positioned_argument(argument_descriptor_t* = nullptr);
        std::expected<std::pair<syntax_expression_result_t, bool>, error_storage> use_next_positioned_argument(expected_result_t const& exp, argument_descriptor_t* = nullptr);

        std::expected<std::pair<syntax_expression_result_t, bool>, error_storage> use_named_argument(identifier name, expected_result_t const& exp, argument_descriptor_t* = nullptr);

        std::expected<std::pair<syntax_expression_result_t, bool>, error_storage> use_next_argument(expected_result_t const& exp, next_argument_descriptor_t* pe);
        named_expression_t unused_argument();

        void reuse_argument(size_t argindex);

    private:
        std::expected<std::pair<syntax_expression_result_t, bool>, error_storage>
        do_resolve(argument_cache& arg_cache, expected_result_t const& exp);
    };

    session new_session(fn_compiler_context&) const;

    local_variable& new_temporary(unit&, identifier, entity_identifier type, semantic::expression_span);
    void export_temporaries(syntax_expression_result&);

    qname_view functional_name() const noexcept;
    qname_identifier functional_id() const noexcept;

private:
    void init_bindings();
};

}
