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

    annotated_string make_string(annotated_string_view) const;
    mp::integer make_integer(string_view) const;
    mp::decimal make_decimal(string_view) const;

    identifier new_identifier() const;
    annotated_identifier make_identifier(annotated_string_view) const;
    
    annotated_qname make_qname(annotated_string_view) const;

    annotated_qname_identifier make_qname_identifier(annotated_qname) const;
    annotated_qname_identifier make_qname_identifier(annotated_string_view, bool is_abs) const;

    //identifier make_required_identifier(string_view);
    //small_u32string make_string(string_view);

    void set_statements(statement_set_t);

    void append_error(std::string errmsg);

    shared_ptr<lex::code_resource> get_resource() const;

    void pop_resource() { resource_stack_.pop_back(); }

    std::expected<statement_set_t, std::string> parse(fs::path const& f);
    std::expected<statement_set_t, std::string> parse_string(string_view);

private:
    std::expected<statement_set_t, std::string> parse(string_view code);

    unit& unit_;

    boost::container::small_vector<shared_ptr<lex::code_resource>, 8> resource_stack_;
    
    boost::container::small_vector<qname, 8> ns_stack_;

    std::vector<std::string> error_messages_;
    
    statement_set_t declarations_;
};

}
