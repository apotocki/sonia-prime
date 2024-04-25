//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <expected>

#include "sonia/variant.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/filesystem.hpp"

#include "ast_terms.hpp"
#include "sonia/logger/logger.hpp"

namespace sonia::lang::bang {

class unit;

class parser_context
{
public:
    explicit parser_context(unit& u)
        : unit_{ u }
    {
        ns_stack_.push_back({});
    }

    qname const& ns() const { return ns_stack_.back(); }

    void push_ns(qname);
    void pop_ns();
    //qname const& ns() const { return ns_stack_.back(); }

    identifier new_identifier();
    annotated_identifier make_identifier(annotated_string_view);
    annotated_string make_string(annotated_string_view);
    int make_int(string_view);
    decimal make_numeric(string_view);

    //identifier make_required_identifier(string_view);
    //small_u32string make_string(string_view);

    void set_declarations(declaration_set_t);

    void append_error(std::string errmsg);

    small_string get_resource() const
    {
        if (resource_stack_.empty()) return small_string{"<code>"sv};
        auto u8str = resource_stack_.back().generic_u8string();
        return small_string{ reinterpret_cast<char const*>(u8str.data()), u8str.size() };
    }

    void pop_resource() { resource_stack_.pop_back(); }

    std::expected<declaration_set_t, std::string> parse(fs::path const& f);
    std::expected<declaration_set_t, std::string> parse(string_view code);

private:
    unit& unit_;

    boost::container::small_vector<fs::path, 8> resource_stack_;
    
    boost::container::small_vector<qname, 8> ns_stack_;

    std::vector<std::string> error_messages_;
    
    declaration_set_t declarations_;
};

}
