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

class parameter_matcher
{
    friend class parameter_type_expression_visitor;

protected:
    pattern_parameter_descriptor descriptor_;

    small_vector<annotated_identifier, 2> internal_names_;

    // built
    //std::vector<syntax_expression_t> concepts;
    //pattern_expression_t constraint;
    //syntax_expression_t constraint;
    bool variadic = false;
    bool is_expresson = true;

public:
    struct postpone_t {};

    explicit parameter_matcher(annotated_identifier name, pattern_parameter_descriptor) noexcept;
    
    inline void push_internal_name(annotated_identifier id) { internal_names_.push_back(std::move(id)); }

    inline bool is_variadic() const noexcept { return variadic; }

    error_storage build(fn_compiler_context&, basic_fn_pattern const&);
    bool is_pattern() const noexcept;

    // returns match weight or error
    std::expected<int, error_storage> try_match(fn_compiler_context& ctx, syntax_expression_t const&, functional_binding_set&, parameter_match_result&) const; // returns weight delta if matched
    
    // returns match weight or postpone or error
    variant<int, postpone_t, error_storage> try_forward_match(fn_compiler_context& ctx, syntax_expression_t const&, functional_binding_set&, parameter_match_result&) const;

    std::expected<entity_identifier, error_storage> apply_binding(fn_compiler_context& ctx, functional_binding_set&) const;

    std::ostream& print(unit const&, std::ostream&) const;
};

class varnamed_matcher : public parameter_matcher
{
    //annotated_identifier variadic_internal_name_;

public:
    explicit varnamed_matcher(annotated_identifier vin, pattern_parameter_descriptor descr)
        : parameter_matcher{ std::move(vin), std::move(descr) }
        //, variadic_internal_name_{ std::move(vin) }
    {}

    std::expected<std::pair<entity_identifier, int>, error_storage> try_match(fn_compiler_context& ctx, syntax_expression_t const&, functional_binding_set&) const;
};

class named_parameter_matcher
{
public:
    class named_parameter_impl : public parameter_matcher
    {
    public:
        using parameter_matcher::parameter_matcher;
    };

    named_parameter_matcher(annotated_identifier enm, annotated_identifier const* inm, pattern_parameter_descriptor descr, size_t index)
        : external_name_{ std::move(enm) }, index_{ index }
    {
        //if (inm) internal_name_.emplace(inm ? );
        impl_ = make_shared<named_parameter_impl>(inm ? *inm : external_name_, std::move(descr));
    }

    inline annotated_identifier const& external_name() const noexcept { return external_name_; }
    inline size_t index() const noexcept { return index_; }

    inline bool is_variadic() const noexcept { return impl_->is_variadic(); }

    std::expected<int, error_storage> try_match(fn_compiler_context& ctx, syntax_expression_t const&, functional_binding_set&, parameter_match_result&) const;

    std::ostream& print(unit const& u, std::ostream& s) const { return impl_->print(u, s); }

private:
    annotated_identifier external_name_;
    //optional<annotated_identifier> internal_name_;
    shared_ptr<named_parameter_impl> impl_;
    size_t index_;
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
    std::vector<std::tuple<parameter_name, parameter_constraint_modifier_t>> parameters_; // in the order of declaration (fn_pure)
    optional<varnamed_matcher> varnamed_matcher_;
    boost::container::small_flat_set<named_parameter_matcher, 8, named_parameter_matcher_less> named_matchers_;
    small_vector<shared_ptr<parameter_matcher>, 8> matchers_; // also for variadic named
    shared_ptr<parameter_matcher> result_matcher_;
    //boost::container::small_flat_set<identifier, 16> local_variables_;

    uint8_t has_varpack_ : 1;

public:
    basic_fn_pattern(fn_compiler_context&, functional const&, fn_pure const&);
    //void build(fn_compiler_context&) const;

    inline qname_identifier fn_qname_id() const noexcept { return fnl_.id(); }
    inline qname_view fn_qname() const noexcept { return fnl_.name(); }

    //optional<size_t> get_local_variable_index(identifier) const;
    named_parameter_matcher const* get_matcher(identifier) const;
    inline bool has_varnamed() const noexcept { return !!varnamed_matcher_; }

    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, pure_call_t const&, annotated_entity_identifier const&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override;

protected:
    void build_scope(fn_compiler_context&, functional_match_descriptor&) const;
};

class runtime_fn_pattern : public basic_fn_pattern
{
public:
    using basic_fn_pattern::basic_fn_pattern;
    error_storage apply(fn_compiler_context&, qname_identifier, functional_match_descriptor&) const override;

    virtual shared_ptr<entity> build(fn_compiler_context&, functional_match_descriptor&, entity_signature&&) const = 0;
};

class generic_fn_pattern: public runtime_fn_pattern
{
    shared_ptr<std::vector<statement>> body_;
    fn_kind kind_;

public:
    generic_fn_pattern(fn_compiler_context&, functional const&, fn_decl_t const&);
    
    
    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context&, qname_identifier, functional_match_descriptor&) const override;

protected:
    shared_ptr<entity> build(fn_compiler_context&, functional_match_descriptor&, entity_signature&&) const override;
};

}
