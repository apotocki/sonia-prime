//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/semantic.hpp"

namespace sonia::lang::bang { class unit; }

namespace sonia::lang::bang::semantic {

class value_printer_visitor : public static_visitor<void>
{
    unit const& u_;
    std::ostream& ss;

public:
    explicit value_printer_visitor(unit const& u, std::ostream& s)
        : u_{ u }, ss{ s } {}

    void operator()(null_t const&) const;
    void operator()(bool) const;
    void operator()(entity_identifier) const;
    void operator()(function_value const& fval) const;
    //void operator()(uint64_t) const;
    //void operator()(mp::integer) const;
    //void operator()(mp::decimal) const;
    //void operator()(small_string) const;

    template <typename T>
    void operator()(std::vector<T> const& vec) const
    {
        ss << '[';
        bool first = true;
        for (T const& v : vec) {
            if (first) first = false;
            else { ss << ", "sv; }
            apply_visitor(*this, v);
        }
        ss << ']';
    }

    template <typename T>
    void operator()(T const& val) const
    {
        ss << val;
    }
};

class expression_printer_visitor : public static_visitor<void>
{
    unit const& u_;
    std::ostream& ss;
    string_view indent;
    mutable size_t indent_cnt;

public:
    explicit expression_printer_visitor(unit const& u, std::ostream& s, string_view indentstr = "  "sv, size_t icnt = 0)
        : u_{ u }, ss{ s }, indent{ indentstr }, indent_cnt{ icnt } {}

    void operator()(empty_t const&) const;
    void operator()(expression_span) const;
    void operator()(expression_list_t const&) const;
    void operator()(push_value const&) const;
    void operator()(push_local_variable const&) const;
    void operator()(invoke_function const&) const;
    void operator()(return_statement const&) const;
    void operator()(truncate_values const&) const;

    void operator()(conditional_t const&) const;

    template <typename T>
    void operator()(T const& se) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }

private:
    void do_indent() const;
};

}
