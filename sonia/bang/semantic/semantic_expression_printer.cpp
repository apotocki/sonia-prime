//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "expression_printer.hpp"

#include "sonia/bang/unit.hpp"
#include "sonia/bang/utility/linked_list.ipp"

namespace sonia::lang::bang::semantic {

void expression_printer_visitor::do_indent() const
{
    for (size_t i = 0; i < indent_cnt; ++i) ss << indent;
}

void expression_printer_visitor::operator()(empty_t const& i) const
{
    do_indent();
    ss << "noop\n"sv;
}

void expression_printer_visitor::operator()(expression_span esp) const
{
    ++indent_cnt;
    while (esp) {
        apply_visitor(*this, esp.front());
        esp.pop_front();
    }
    --indent_cnt;
}

void expression_printer_visitor::operator()(expression_list_t const& evec) const
{
    ++indent_cnt;
    for (expression const& e : evec) {
        apply_visitor(*this, e);
    }
    --indent_cnt;
}

void expression_printer_visitor::operator()(conditional_t const& c) const
{
    do_indent();
    if (c.true_branch) {
        ss << "if true\n"sv;
        this->operator()(c.true_branch);
    }
    if (c.false_branch) {
        ss << "if false\n"sv;
        this->operator()(c.false_branch);
    }
}

void expression_printer_visitor::operator()(push_value const& v) const
{
    do_indent();
    ss << "push "sv;
    value_printer_visitor vis{ u_, ss };
    apply_visitor(vis, v.value);
    ss << '\n';
}

void expression_printer_visitor::operator()(push_local_variable const& lv) const
{
    do_indent();
    ss << "push VAR("sv << lv.varid.value;
#ifdef SONIA_LANG_DEBUG
    ss << ") "sv << u_.print(lv.debug_name) << '\n';
#else
    ss << ")\n"sv;
#endif
}

void expression_printer_visitor::operator()(invoke_function const& f) const
{
    do_indent();
    ss << "invoke "sv << f.fn << '\n';
}

void expression_printer_visitor::operator()(return_statement const&) const
{
    do_indent();
    ss << "return\n"sv;
}

void expression_printer_visitor::operator()(truncate_values const& tv) const
{
    do_indent();
    ss << "truncate count: "sv << tv.count << ", keep back: "sv << tv.keep_back;
}

void value_printer_visitor::operator()(null_t const&) const
{
    ss << "nil"sv;
}

void value_printer_visitor::operator()(bool v) const
{
    ss << (v ? "true"sv : "false"sv);
}

void value_printer_visitor::operator()(entity_identifier eid) const
{
    ss << u_.print(eid);
}

void value_printer_visitor::operator()(function_value const& fval) const
{
    ss << "fn"sv << u_.print(fval.mangled_name);
}

}
