//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "errors.hpp"

#include "errors/utility.hpp"

namespace sonia::lang::bang {

void error::rethrow(unit& u) const
{
    throw exception{ u.print(*this) };
}

std::string error_printer_visitor::print_general(lex::resource_location const& loc, string_view errstr, string_view object, lex::resource_location const* optseeloc)
{
    std::ostringstream errss;
    if (loc) {
        write(errss, loc) << ": "sv;
    }
    if (!object.empty()) {
        errss << '`' << object << "`: "sv;
    }
    errss << errstr;
    if (optseeloc) {
        errss << ", see declaration at "sv;
        write(errss, *optseeloc);
    }

    return errss.str();
    //if (object.empty() && !optseeloc) {
    //    return ("%1%(%2%,%3%): %4%"_fmt % loc.resource % loc.line % loc.column % errstr).str();
    //} else if (object.empty() && optseeloc) {
    //    return ("%1%(%2%,%3%): %4%, see declaration at %5%(%6%,%7%)"_fmt 
    //        % loc.resource % loc.line % loc.column % errstr
    //        % optseeloc->resource % optseeloc->line % optseeloc->column
    //    ).str();
    //} else if (!object.empty() && !optseeloc) {
    //    return ("%1%(%2%,%3%): `%4%`: %5%"_fmt % loc.resource % loc.line % loc.column % object % errstr).str();
    //} else {
    //    return ("%1%(%2%,%3%): `%4%`: %5%, see declaration at %6%(%7%,%8%)"_fmt
    //        % loc.resource % loc.line % loc.column % object % errstr
    //        % optseeloc->resource % optseeloc->line % optseeloc->column
    //    ).str();
    //}
}

void error_printer_visitor::operator()(general_error const& err)
{
    s_ << print_general(
        err.location(),
        apply_visitor(string_resolver_visitor{}, err.description(u_)),
        apply_visitor(string_resolver_visitor{}, err.object(u_)),
        err.ref_location()
    );
}

void error_printer_visitor::operator()(binary_relation_error const& err)
{
    if (err.location()) {
        write(s_, err.location()) << ": "sv;
    }
    s_ << '`' <<
        apply_visitor(string_resolver_visitor{}, err.left_object(u_)) <<
        "` and `"sv <<
        apply_visitor(string_resolver_visitor{}, err.right_object(u_)) <<
        "` : "sv;
    
    s_ << apply_visitor(string_resolver_visitor{}, err.description(u_));
    if (auto* ploc = err.ref_location()) {
        s_ << ", see declaration at "sv;
        write(s_, *ploc);
    }
}

error::string_t binary_relation_error::left_object(unit const& u) const noexcept
{
    return apply_visitor(printer_resolver_visitor{ u }, left_);
}

error::string_t binary_relation_error::right_object(unit const& u) const noexcept
{
    return apply_visitor(printer_resolver_visitor{ u }, right_);
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
            % u.print(from_)
            % u.print(to_)
        ).str();
    } else {
        return ("cannot convert to `%1%`"_fmt 
            % u.print(to_)
        ).str();
    }
}

#if 0
general_error::string_t unknown_case_error::object(unit const& u) const noexcept
{
    return u.print(ci_.name.value);
}

general_error::string_t unknown_case_error::description(unit const& u) const noexcept
{
    return ("is not a case of the enumerration %1%"_fmt % u.print(enum_name_)).str();
}
#endif

general_error::string_t undeclared_identifier_error::object(unit const& u) const noexcept
{
    return u.print(idname_.value);
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
            s_ << "\n and \n";
        } else { first = false; }
        e->visit(*this);
    }
}

std::string ambiguity_error::object(unit const& u) const noexcept
{
    return u.print(functional_.value);
}


void error_printer_visitor::operator()(ambiguity_error const& err)
{
    s_ << print_general(err.location(), "ambiguity call error"sv, err.object(u_));
    s_ << "\ncould be:\n";
    bool first = true;
    for (auto const& e : err.alternatives()) {
        if (!first) {
            s_ << "\n or \n";
        }
        else { first = false; }
        if (e.location) {
            u_.print_to(s_, e.location) << ": ";
        }
        u_.print_to(s_, e.sig);
        if (!e.location) {
            s_ << ", aka "sv << e.description;
        }
    }
}


general_error::string_t parameter_not_found_error::description(unit const& u) const noexcept
{
    return ("parameter `%1%` of `%2%` is not found"_fmt %
        u.print(param.value) % u.print(entity_name)).str();
}

//void error_printer_visitor::operator()(parameter_not_found_error const& err)
//{
//    s_ << ("parameter `%1%` of `%2%` is not found"_fmt % u_.print(err.param.value) % u_.print(err.entity_name)).str();
//}

}
