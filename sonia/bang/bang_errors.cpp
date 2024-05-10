//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "errors.hpp"
#include "unit.hpp"

namespace sonia::lang::bang {

std::string error_printer_visitor::print_general(lex::resource_location const& loc, string_view errstr, string_view object, lex::resource_location const* optseeloc)
{
    if (object.empty() && !optseeloc) {
        return ("%1%(%2%,%3%): %4%"_fmt % loc.resource % loc.line % loc.column % errstr).str();
    } else if (object.empty() && optseeloc) {
        return ("%1%(%2%,%3%): %4%, see declaration at %5%(%6%,%7%)"_fmt 
            % loc.resource % loc.line % loc.column % errstr
            % optseeloc->resource % optseeloc->line % optseeloc->column
        ).str();
    } else if (!object.empty() && !optseeloc) {
        return ("%1%(%2%,%3%): `%4%`: %5%"_fmt % loc.resource % loc.line % loc.column % object % errstr).str();
    } else {
        return ("%1%(%2%,%3%): `%4%`: %5%, see declaration at %6%(%7%,%8%)"_fmt
            % loc.resource % loc.line % loc.column % object % errstr
            % optseeloc->resource % optseeloc->line % optseeloc->column
        ).str();
    }
}

struct printer_resolver_visitor : static_visitor<std::string>
{
    unit const& u_;
    explicit printer_resolver_visitor(unit const& u) : u_{ u } {}

    inline result_type operator()(null_t const&) const { return {}; }

    template <typename T>
    inline result_type operator()(T const& val) const
    {
        return u_.print(val);
    }
};

struct string_resolver_visitor : static_visitor<string_view>
{
    string_resolver_visitor() = default;

    inline result_type operator()(std::string const& str) const { return str; }
    inline result_type operator()(string_view str) const { return str; }
};

void error_printer_visitor::operator()(general_error const& err)
{
    s_ << print_general(
        err.location(),
        apply_visitor(string_resolver_visitor{}, err.description(u_)),
        apply_visitor(string_resolver_visitor{}, err.object(u_)),
        err.see_location()
    );
}

general_error::string_t basic_general_error::object(unit const& u) const noexcept
{ 
    return apply_visitor(printer_resolver_visitor{u}, object_);
}

general_error::string_t cast_error::object(unit const& u) const noexcept
{
    if (expr_) return u.print(*expr_);
    return ""sv;
}

general_error::string_t cast_error::description(unit const& u) const noexcept
{
    if (from_) {
        return ("cannot convert from `%1%` to `%2%`"_fmt 
            % u.print(*from_)
            % u.print(to_)
        ).str();
    } else {
        return ("cannot convert to `%1%`"_fmt 
            % u.print(to_)
        ).str();
    }
}

general_error::string_t unknown_case_error::object(unit const& u) const noexcept
{
    return u.print(ce_.name.value);
}

general_error::string_t unknown_case_error::description(unit const& u) const noexcept
{
    return ("is not a case of the enumerration %1%"_fmt % u.print(enum_name_)).str();
}

general_error::string_t undeclared_identifier_error::object(unit const& u) const noexcept
{
    return u.print(idname_.value);
}

general_error::string_t identifier_redefinition_error::object(unit const& u) const noexcept
{
    return apply_visitor(printer_resolver_visitor{ u }, name_);
}

general_error::string_t left_not_an_object_error::description(unit const& u) const noexcept
{
    return ("left of `.%1%` must be object, type is `%2%`"_fmt %
        u.print(right_) % u.print(type_)).str();
}

general_error::string_t wrong_lvalue_error::object(unit const& u) const noexcept
{
    return u.print(expr_);
}

void error_printer_visitor::operator()(alt_error const& err)
{
    bool first = true;
    for (auto const &e : err.alternatives) {
        if (!first) {
            s_ << "\n or \n";
        } else { first = false; }
        e->visit(*this);
    }
}

general_error::string_t parameter_not_found_error::description(unit const& u) const noexcept
{
    return ("parameter `%1%` of `%2%` is not found"_fmt %
        u.print(param.value) % u.print(entity_name)).str();
}

general_error::string_t function_call_match_error::object(unit const& u) const noexcept
{
    return u.print(functional_.value);
}

general_error::string_t function_call_match_error::description(unit const& u) const noexcept
{
    std::ostringstream ss;
    ss << "can't match the function signature: "sv;
    ss << u.print(signature_.fn_type);
    if (reason_) {
        ss << ", caused by: \n" << u.print(*reason_);
    }
    return ss.str();
}

//void error_printer_visitor::operator()(parameter_not_found_error const& err)
//{
//    s_ << ("parameter `%1%` of `%2%` is not found"_fmt % u_.print(err.param.value) % u_.print(err.entity_name)).str();
//}

}
