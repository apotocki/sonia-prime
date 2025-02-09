//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <expected>

#include "sonia/variant.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/filesystem.hpp"
#include "sonia/small_vector.hpp"

#include "ast_terms.hpp"
#include "sonia/logger/logger.hpp"

namespace sonia::lang::bang {

class unit;

class parser_context
{
public:
    inline explicit parser_context(unit& u) noexcept
        : unit_{ u }
        , statements_{ u }
    {}

    annotated_string make_string(annotated_string_view) const;
    mp::integer make_integer(string_view) const;
    mp::decimal make_decimal(string_view) const;

    identifier new_identifier() const;
    annotated_identifier make_identifier(annotated_string_view) const;
    
    annotated_qname make_qname(annotated_string_view) const;

    annotated_qname_identifier make_qname_identifier(annotated_qname) const;
    annotated_qname_identifier make_qname_identifier(annotated_string_view, bool is_abs) const;

    managed_statement_list new_statement_list() const;
    statement_span push(managed_statement_list&&);

    //identifier make_required_identifier(string_view);
    //small_u32string make_string(string_view);

    void set_root_statements(managed_statement_list&&);
    inline managed_statement_list const& statements() const noexcept { return statements_; }
    inline managed_statement_list& statements() noexcept { return statements_; }

    void append_error(std::string errmsg);

    inline shared_ptr<lex::code_resource> get_resource() const noexcept { return resource_; }

    std::expected<statement_span, std::string> parse(fs::path const& f, fs::path const* base_path = nullptr);
    std::expected<statement_span, std::string> parse_string(string_view);

private:
    std::expected<statement_span, std::string> parse(string_view code);

    unit& unit_;

    shared_ptr<lex::code_resource> resource_;
    
    std::vector<std::string> error_messages_;
    
    statement_span root_statements_;
    managed_statement_list statements_;
};

}
