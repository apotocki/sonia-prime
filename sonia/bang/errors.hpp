//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>

#include "semantic.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

namespace sonia::lang::bang {

class error;
class general_error;
class undeclared_identifier_error;
class identifier_redefinition_error;

class cast_error;
class unknown_case_error;
class left_not_an_object_error;
class wrong_lvalue_error;

class function_call_match_error;
class parameter_not_found_error;
class alt_error;
class ambiguity_error;
class circular_dependency_error;

class error_visitor
{
public:
    virtual ~error_visitor() = default;
    virtual void operator()(general_error const&) = 0;
    virtual void operator()(alt_error const&) = 0;
    virtual void operator()(ambiguity_error const&) = 0;
    virtual void operator()(circular_dependency_error const&) = 0;
};

class error
{
    shared_ptr<error> cause_;

public:
    virtual ~error() = default;
    virtual void visit(error_visitor&) const = 0;

    inline void set_cause(shared_ptr<error> cause) noexcept { cause_ = std::move(cause); }
    inline shared_ptr<error> const& cause() const { return cause_; }

    [[noreturn]] virtual void rethrow(unit&) const;
};

using error_storage = shared_ptr<error>;

template <std::derived_from<error> T, typename ... Args>
inline error_storage make_error(Args&& ... args) { return sonia::make_shared<T>(std::forward<Args>(args) ...); }
inline error_storage append_cause(error_storage err, error_storage cause)
{
    err->set_cause(std::move(cause));
    return err;
}

class alt_error : public error
{
public:
    std::vector<error_storage> alternatives;
    void visit(error_visitor& vis) const override { vis(*this); }
};

class ambiguity_error : public error
{
public:
    struct alternative
    {
        lex::resource_location location;
        entity_signature sig;
    };

    explicit ambiguity_error(annotated_qname_identifier f, std::vector<alternative> as)
        : functional_{ f }, alternatives_{ std::move(as) }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept { return functional_.location; }
    std::string object(unit const&) const noexcept;
    inline span<const alternative> alternatives() const noexcept { return alternatives_; }

private:
    annotated_qname_identifier functional_;
    std::vector<alternative> alternatives_;
};


class general_error : public error
{
protected:
    using string_t = variant<std::string, string_view>;

public:
    virtual lex::resource_location const& location() const = 0;
    virtual string_t object(unit const&) const = 0;
    virtual string_t description(unit const&) const = 0;
    virtual lex::resource_location const* ref_location() const noexcept { return nullptr; }
};

class basic_general_error : public general_error
{
protected:
    using object_t = variant<null_t, syntax_expression_t, qname, qname_view, qname_identifier, entity_identifier, identifier>;

    lex::resource_location location_;
    lex::resource_location reflocation_;
    string_t description_;
    object_t object_;

public:
    basic_general_error(lex::resource_location loc, string_t descr, object_t obj = null_t{}, lex::resource_location refloc = {})
        : location_{ std::move(loc) }, description_{ descr }, object_{ std::move(obj) }, reflocation_{ std::move(refloc) }
    {}

    basic_general_error(error_context const& errctx, string_t descr)
        : location_{ errctx.location() }, description_{ descr }, reflocation_{ errctx.refloc }
    {
        if (auto optexpr = errctx.expression(); optexpr) {
            object_ = *optexpr;
        }
    }

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return location_; }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override { return description_; }
    lex::resource_location const* ref_location() const noexcept override { return reflocation_ ? &reflocation_ : nullptr; }
};

class undeclared_identifier_error : public general_error
{
    annotated_qname idname_;

public:
    inline explicit undeclared_identifier_error(annotated_qname idname) noexcept
        : idname_{ std::move(idname) }
    {}

    undeclared_identifier_error(lex::resource_location loc, qname_view idname)
        : idname_{ qname{idname}, std::move(loc) }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return idname_.location; }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override { return "undeclared identifier"sv; }
};



class parameter_not_found_error : public general_error
{
public:
    annotated_qname_identifier param;
    qname_identifier entity_name;
    parameter_not_found_error(qname_identifier qn, annotated_qname_identifier p)
        : param{ std::move(p) }, entity_name{ qn }
    {}
    //parameter_not_found_error(qname_view qn, annotated_identifier p)
    //    : param{ qname{std::move(p.value)}, std::move(p.location) }, entity_name{ qn }
    //{}
    void visit(error_visitor& vis) const override { vis(*this); }
    lex::resource_location const& location() const noexcept override { return param.location; }
    string_t object(unit const&) const noexcept override { return ""sv; }
    string_t description(unit const&) const noexcept override;
};

class cast_error : public general_error
{
public:
    lex::resource_location location_;
    optional<syntax_expression_t> expr_;
    entity_identifier from_;
    entity_identifier to_;
    lex::resource_location refloc_;

    cast_error(lex::resource_location loc, entity_identifier to, entity_identifier from = {}, optional<syntax_expression_t> expr = nullopt)
        : location_{ std::move(loc) }, from_{std::move(from)}, to_{ std::move(to) }, expr_{ std::move(expr) }
    {}

    cast_error(error_context const& errctx, entity_identifier to, entity_identifier from = {})
        : location_{ errctx.location() }, from_{ std::move(from) }, to_{ std::move(to) }, expr_{ errctx.expression() }, refloc_{ errctx.refloc }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return location_; }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override;
    lex::resource_location const* ref_location() const noexcept override { return refloc_ ? &refloc_ : nullptr; }
};

class unknown_case_error : public general_error
{
public:
    context_identifier ci_;
    qname_identifier enum_name_;
    unknown_case_error(context_identifier const& ci, qname_identifier enum_name)
        : ci_{ ci }, enum_name_{ enum_name }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return ci_.name.location; }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override;
};

class left_not_an_object_error : public general_error
{
    lex::resource_location location_;
    identifier right_;
    entity_identifier type_;

public:
    left_not_an_object_error(lex::resource_location loc, identifier right, entity_identifier type)
        : location_{ std::move(loc) }, right_{ right }, type_{ std::move(type) }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return location_; }
    string_t object(unit const&) const noexcept override { return ""sv; }
    string_t description(unit const&) const noexcept override;
};

class wrong_lvalue_error : public general_error
{
    syntax_expression_t expr_;

public:
    explicit wrong_lvalue_error(syntax_expression_t const& expr)
        : expr_{ expr }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return get_start_location(expr_); }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override { return "is not rvalue"sv; }
};

class error_printer_visitor : public error_visitor
{
    unit const& u_;
    std::ostream & s_;

public:
    error_printer_visitor(unit const& u, std::ostream& s) : u_{u}, s_{s} {}

    void operator()(alt_error const&) override;
    //void operator()(parameter_not_found_error const&) override;
    void operator()(general_error const&) override;

    void operator()(ambiguity_error const&) override;
    void operator()(circular_dependency_error const&) override;

private:
    static std::string print_general(lex::resource_location const& loc, string_view err, string_view object, lex::resource_location const* optseeloc = nullptr);
};

}
