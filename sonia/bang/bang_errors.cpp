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

#if 0
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

std::string ambiguity_error::object(unit const& u) const noexcept
{
    return u.print(functional_.value);
}

std::ostream& error_printer_visitor::print_general(error::location_t const& loc, string_view errstr, string_view object, lex::resource_location const* optseeloc)
{
    bool need_indent = true;
    apply_visitor(make_functional_visitor<void>([this, &need_indent](auto const& l) {
        if constexpr (std::is_same_v<lex::resource_location, std::decay_t<decltype(l)>>) {
            if (!l) return;
            u_.print_to(s_, l, indent());
        } else {
            s_ << indent() << l;
        }
        need_indent = false;
    }), loc);

    if (!object.empty()) {
        if (need_indent) {
            s_ << indent();
            need_indent = false;
        }
        s_ << '`' << object << "`: "sv;
    }
    if (need_indent) s_ << indent();
    s_ << errstr;
    if (optseeloc) {
        s_ << '\n' << indent() << ": see "sv;
        optseeloc->print_to(s_, indent(), lex::resource_print_mode_t::just_pointer);
    }

    return s_;
}

void error_printer_visitor::operator()(general_error const& err)
{
    print_general(
        err.location(),
        apply_visitor(string_resolver_visitor{}, err.description(u_)),
        apply_visitor(string_resolver_visitor{}, err.object(u_)),
        err.ref_location()
    );
    
    if (auto cause = err.cause()) {
        s_ << '\n' << indent() << "caused by: \n"sv;
        inc_indent();
        cause->visit(*this);
        dec_indent();
    }
}

void error_printer_visitor::operator()(binary_relation_error const& err)
{
    if (err.location()) {
        u_.print_to(s_, err.location(), indent()) << ": "sv;
    }
    s_ << '`' <<
        apply_visitor(string_resolver_visitor{}, err.left_object(u_)) <<
        "` and `"sv <<
        apply_visitor(string_resolver_visitor{}, err.right_object(u_)) <<
        "` : "sv;
    
    s_ << apply_visitor(string_resolver_visitor{}, err.description(u_));
    if (auto* ploc = err.ref_location()) {
        u_.print_to(s_ << ", see declaration at "sv, *ploc, indent());
    }
}

void error_printer_visitor::operator()(alt_error const& err)
{
    bool first = true;
    for (auto const& e : err.alternatives) {
        if (!first) {
            s_ << '\n' << indent() << " and \n"sv;
        }
        else { first = false; }
        e->visit(*this);
    }
}

void error_printer_visitor::operator()(ambiguity_error const& err)
{
    print_general(err.location(), "ambiguity call error"sv, err.object(u_));
    s_ << '\n' << indent() << "could be:\n"sv;
    bool first = true;
    for (auto const& e : err.alternatives()) {
        if (!first) {
            s_ << '\n' << indent() << " or \n"sv;
        }
        else { first = false; }
        if (e.location) {
            u_.print_to(s_, e.location, indent()) << ": ";
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
