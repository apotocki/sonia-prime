//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>

#include "semantic.hpp"

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


class error_visitor
{
public:
    virtual ~error_visitor() = default;
    virtual void operator()(general_error const&) = 0;
    virtual void operator()(alt_error const&) = 0;
    virtual void operator()(parameter_not_found_error const&) = 0;
};

class error
{
public:
    virtual ~error() = default;
    virtual void visit(error_visitor&) const = 0;
};

//using error_storage = automatic_polymorphic<error, 8 * sizeof(void*)>;
using error_storage = shared_ptr<error>;

template <std::derived_from<error> T, typename ... Args>
error_storage make_error(Args&& ... args) { return sonia::make_shared<T>(std::forward<Args>(args) ...); }

class alt_error : public error
{
public:
    std::vector<error_storage> alternatives;
    void visit(error_visitor& vis) const override { vis(*this); }
};

class general_error : public error
{
protected:
    using string_t = variant<std::string, string_view>;

public:
    virtual lex::resource_location const& location() const = 0;
    virtual string_t object(unit const&) const = 0;
    virtual string_t description(unit const&) const = 0;
    virtual lex::resource_location const* see_location() const { return nullptr; }
};

class basic_general_error : public general_error
{
protected:
    using object_t = variant<null_t, expression_t, qname_view, identifier>;

    lex::resource_location location_;
    string_t description_;
    object_t object_;
    optional< lex::resource_location> refloc_;

public:
    basic_general_error(lex::resource_location loc, string_t descr, object_t obj = null_t{})
        : location_{std::move(loc)}, description_{descr}, object_{std::move(obj)}
    {}

    basic_general_error(error_context const& errctx, string_t descr)
        : location_{ errctx.location() }, description_{ descr }, refloc_{ errctx.refloc }
    {
        if (auto optexpr = errctx.expression(); optexpr) {
            object_ = *optexpr;
        }
    }

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return location_; }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override { return description_; }
    lex::resource_location const* see_location() const noexcept override { return refloc_ ? &*refloc_ : nullptr; }
};

class undeclared_identifier_error : public general_error
{
    lex::resource_location location_;
    qname_view idname_;

public:
    undeclared_identifier_error(lex::resource_location loc, qname_view idname)
        : location_{ std::move(loc) }, idname_{ idname }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return location_; }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override { return "undeclared identifier"sv; }
};

class general_with_see_location_error : public basic_general_error
{
    lex::resource_location seelocation_;
public:
    general_with_see_location_error(lex::resource_location loc, string_t descr, lex::resource_location seeloc, object_t obj = null_t{})
        : basic_general_error{ std::move(loc), descr, std::move(obj) }, seelocation_{ std::move(seeloc) }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const* see_location() const noexcept override { return &seelocation_; }
};

class identifier_redefinition_error : public general_error
{
    lex::resource_location location_;
    lex::resource_location seelocation_;
    using object_t = variant<qname_view, identifier>;
    object_t name_;

public:
    identifier_redefinition_error(lex::resource_location loc, lex::resource_location seeloc, object_t qn)
        : location_{ std::move(loc) }, seelocation_{ std::move(seeloc) }, name_{ qn }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return location_; }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override { return "identifier redefinition"sv; }
    lex::resource_location const* see_location() const override { return &seelocation_; }
};

class cast_error : public general_error
{
public:
    lex::resource_location location_;
    optional<expression_t> expr_;
    optional<bang_type> from_;
    bang_type to_;
    optional<lex::resource_location> refloc_;

    cast_error(lex::resource_location loc, bang_type to, optional<bang_type> from = nullopt, optional<expression_t> expr = nullopt)
        : location_{ std::move(loc) }, from_{std::move(from)}, to_{ std::move(to) }, expr_{ std::move(expr) }
    {}

    cast_error(error_context const& errctx, bang_type to, optional<bang_type> from = nullopt)
        : location_{ errctx.location() }, from_{ std::move(from) }, to_{ std::move(to) }, expr_{ errctx.expression() }, refloc_{ errctx.refloc }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return location_; }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override;
    lex::resource_location const* see_location() const noexcept override { return refloc_ ? &*refloc_ : nullptr; }
};

class unknown_case_error : public general_error
{
public:
    case_expression ce_;
    qname_view enum_name_;
    unknown_case_error(case_expression const& ce, qname_view enum_name)
        : ce_{ ce }, enum_name_{ enum_name }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return ce_.name.location; }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override;
};

class left_not_an_object_error : public general_error
{
    lex::resource_location location_;
    identifier right_;
    bang_type type_;

public:
    left_not_an_object_error(lex::resource_location loc, identifier right, bang_type type)
        : location_{ std::move(loc) }, right_{ right }, type_{ std::move(type) }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return location_; }
    string_t object(unit const&) const noexcept override { return ""sv; }
    string_t description(unit const&) const noexcept override;
};

class wrong_lvalue_error : public general_error
{
    expression_t expr_;

public:
    explicit wrong_lvalue_error(expression_t const& expr)
        : expr_{ expr }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return get_start_location(expr_); }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override { return "is not rvalue"sv; }
};

class function_call_match_error : public general_error
{
public:
    lex::resource_location location_;

    function_call_match_error(lex::resource_location l) : location_{ l } {}
    void visit(error_visitor& vis) const override { vis(*this); }

    lex::resource_location const& location() const noexcept override { return location_; }
    string_t object(unit const&) const noexcept override { return ""sv; }
    string_t description(unit const&) const noexcept override { return "can't match the function call"sv; }
};

class parameter_not_found_error : public error
{
public:
    annotated_identifier param;
    qname_view entity_name;
    parameter_not_found_error(qname_view qn, annotated_identifier p)
        : param{ std::move(p) }, entity_name{ qn }
    {}
    void visit(error_visitor& vis) const override { vis(*this); }
};

class error_printer_visitor : public error_visitor
{
    unit& u_;
    std::ostream & s_;

public:
    error_printer_visitor(unit& u, std::ostream& s) : u_{u}, s_{s} {}

    void operator()(alt_error const&) override;
    void operator()(parameter_not_found_error const&) override;
    void operator()(general_error const&) override;

private:
    static std::string print_general(lex::resource_location const& loc, string_view err, string_view object, lex::resource_location const* optseeloc = nullptr);
};

}
