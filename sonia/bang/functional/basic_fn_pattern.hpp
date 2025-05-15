//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/shared_ptr.hpp"
#include "sonia/bang/entities/functional.hpp"

#include "sonia/small_vector.hpp"
#include <boost/container/flat_set.hpp>

namespace sonia::lang::bang {

class basic_fn_pattern;
class parameter_type_expression_visitor;
class internal_function_entity;

class parameter_matcher
{
    friend class parameter_type_expression_visitor;

protected:
    small_vector<annotated_identifier, 2> internal_names_;
    parameter_constraint_modifier_t modifier_;
    parameter_constraint_set_t constraints_;

    // built
    //std::vector<syntax_expression_t> concepts;
    //pattern_expression_t constraint;
    //syntax_expression_t constraint;
    bool variadic = false;
    bool is_expresson = true;

public:
    struct postpone_t {};
    struct ignore_t {}; // for the case when argument is void

    explicit parameter_matcher(annotated_identifier, parameter_constraint_modifier_t, parameter_constraint_set_t) noexcept;
    
    inline void push_internal_name(annotated_identifier id) { internal_names_.push_back(std::move(id)); }

    inline bool is_variadic() const noexcept { return variadic; }

    error_storage build(fn_compiler_context&, basic_fn_pattern const&);
    bool is_pattern() const noexcept;

    // returns match weight or ignore or error
    variant<int, ignore_t, error_storage> try_match(fn_compiler_context& caller_ctx, fn_compiler_context& callee_ctx, syntax_expression_t const&, functional_binding&, parameter_match_result&, semantic::expression_list_t&) const; // returns weight delta if matched
    
    // returns match weight or ignore or postpone or error
    variant<int, ignore_t, postpone_t, error_storage> try_forward_match(fn_compiler_context& caller_ctx, fn_compiler_context& callee_ctx, syntax_expression_t const&, functional_binding_set&, parameter_match_result&, semantic::expression_list_t&) const;

    std::ostream& print(unit const&, std::ostream&) const;

protected:
    void bind_names(span<const annotated_identifier> names, field_descriptor const& type_or_value, functional_binding&) const;
    virtual void update_binding(unit&, field_descriptor const& type_or_value, functional_binding&) const;
};

class named_parameter_matcher : public parameter_matcher
{
public:
    inline named_parameter_matcher(annotated_identifier enm, annotated_identifier const* inm, parameter_constraint_modifier_t mod, parameter_constraint_set_t cs)
        : parameter_matcher{ inm ? *inm : enm, mod, std::move(cs) }
        , external_name_{ enm }
    {}

    inline annotated_identifier const& external_name() const noexcept { return external_name_; }

private:
    annotated_identifier external_name_;
};

class varnamed_parameter_matcher : public parameter_matcher
{
public:
    inline varnamed_parameter_matcher(annotated_identifier vin, parameter_constraint_modifier_t mod, parameter_constraint_set_t cs)
        : parameter_matcher{ std::move(vin), mod, std::move(cs) }
    {}

    void update_binding(unit&, field_descriptor const&, functional_binding&) const override;
    
    variant<int, ignore_t, error_storage> try_match(fn_compiler_context& caller_ctx, fn_compiler_context& callee_ctx, annotated_identifier, syntax_expression_t const&, functional_binding&, parameter_match_result&, semantic::expression_list_t&) const;
};

struct named_parameter_matcher_less
{
    using is_transparent = void*;

    inline bool operator() (named_parameter_matcher const& l, named_parameter_matcher const& r) const noexcept
    {
        return l.external_name().value < r.external_name().value;
    }

    inline bool operator() (named_parameter_matcher const& l, identifier const& r) const noexcept
    {
        return l.external_name().value < r;
    }

    inline bool operator() (identifier const& l, named_parameter_matcher const& r) const noexcept
    {
        return  l < r.external_name().value;
    }
};

class basic_fn_pattern : public functional::pattern
{
protected:
    functional const& fnl_;

    struct parameter_descriptor
    {
        annotated_identifier ename;
        small_vector<annotated_identifier, 2> inames;
        optional<syntax_expression_t> default_value;
        parameter_constraint_modifier_t modifier;
        bool is_varnamed;
    };

    // in the order of declaration (fn_pure)
    small_vector<parameter_descriptor, 8> parameters_;

    optional<varnamed_parameter_matcher> varnamed_matcher_;
    boost::container::small_flat_set<named_parameter_matcher, 8, named_parameter_matcher_less> named_matchers_;
    small_vector<shared_ptr<parameter_matcher>, 8> matchers_; // also for variadic named
    
public:
    explicit basic_fn_pattern(functional const&) noexcept;
    error_storage init(fn_compiler_context&, parameter_list_t const& parameters);
    error_storage init(fn_compiler_context&, fn_pure_t const& fnd);
    //void build(fn_compiler_context&) const;

    inline qname_identifier fn_qname_id() const noexcept { return fnl_.id(); }
    inline qname_view fn_qname() const noexcept { return fnl_.name(); }

    optional<std::pair<parameter_constraint_set_t, parameter_constraint_modifier_t>> result_constraints;


    //optional<size_t> get_local_variable_index(identifier) const;
    named_parameter_matcher const* get_matcher(identifier) const;
    inline bool has_varnamed() const noexcept { return !!varnamed_matcher_; }

    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, prepared_call const&, annotated_entity_identifier const&) const override;

    std::expected<syntax_expression_result_t, error_storage> apply(fn_compiler_context&, semantic::expression_list_t&, functional_match_descriptor&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override;

protected:
    void build_scope(unit&, functional_match_descriptor&, internal_function_entity& /* out */) const;
    std::pair<semantic::expression_span, size_t> apply_arguments(fn_compiler_context&, functional_match_descriptor&, semantic::expression_list_t&) const;

    virtual std::pair<semantic::expression_span, size_t> apply_argument(unit&, parameter_match_result& pmr, semantic::expression_list_t&) const;
    std::pair<semantic::expression_span, size_t> apply_mut_argument(parameter_match_result&, semantic::expression_list_t&) const;
    std::pair<semantic::expression_span, size_t> apply_any_argument(unit&, parameter_match_result&, semantic::expression_list_t&) const;

    // builds entity that represents the function
    virtual shared_ptr<entity> build(fn_compiler_context&, functional_match_descriptor&, entity_signature&&) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern::build");
    }

private:
    uint8_t has_varpack_ : 1;
};

class functional_pattern : public basic_fn_pattern
{
    statement_span body_;

public:
    using basic_fn_pattern::basic_fn_pattern;
    inline void set_body(statement_span body) noexcept { body_ = body; }

    
};

class runtime_fn_pattern : public basic_fn_pattern
{
public:
    using basic_fn_pattern::basic_fn_pattern;
    //error_storage apply(fn_compiler_context&, functional_match_descriptor&) const override;

protected:
    
};

class generic_fn_pattern: public runtime_fn_pattern
{
    statement_span body_;
    fn_kind kind_;

public:
    explicit generic_fn_pattern(functional const&);
    
    error_storage init(fn_compiler_context& ctx, fn_decl_t const& fnd);

    //std::expected<application_result_t, error_storage> apply(fn_compiler_context&, functional_match_descriptor&) const override;

protected:
    shared_ptr<entity> build(fn_compiler_context&, functional_match_descriptor&, entity_signature&&) const override;
};

}
