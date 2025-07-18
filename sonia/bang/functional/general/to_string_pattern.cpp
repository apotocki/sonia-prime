//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "to_string_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> to_string_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    auto call_session = call.new_session(ctx);
    auto arg = call_session.use_next_positioned_argument();
    if (!arg) {
        if (!arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing argument"sv));
        }
        return std::unexpected(arg.error());
    }
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    auto pmd = make_shared<functional_match_descriptor>(call);
    pmd->emplace_back(0, arg->first);
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

class to_string_visitor : public entity_visitor
{
    unit& u_;
    std::ostream& oss_;

public:
    inline explicit to_string_visitor(unit& u, std::ostream& os) noexcept 
        : u_{ u }
        , oss_{ os }
    {}

    void operator()(const entity& ent) const override
    {
        ent.print_to(oss_, u_);
    }
    
    // Specialized handlers for literal entities
    void operator()(const generic_literal_entity& ent) const override
    {
        switch (static_cast<builtin_eid>(ent.get_type().value)) {
        case builtin_eid::boolean:
            oss_ << (ent.value().as<bool>() ? "true" : "false");
            return;
        case builtin_eid::integer:
            oss_ << ent.value().as<mp::integer_view>();
            return;
        case builtin_eid::decimal:
            oss_ << ent.value().as<mp::decimal_view>();
            return;
        case builtin_eid::string:
            oss_ << ent.value().as<string_view>();
            return;
        case builtin_eid::i8:
            oss_ << static_cast<int>(ent.value().as<int8_t>());
            return;
        case builtin_eid::u8:
            oss_ << static_cast<unsigned int>(ent.value().as<uint8_t>());
            return;
        case builtin_eid::i16:
            oss_ << ent.value().as<int16_t>();
            return;
        case builtin_eid::u16:
            oss_ << ent.value().as<uint16_t>();
            return;
        case builtin_eid::i32:
            oss_ << ent.value().as<int32_t>();
            return;
        case builtin_eid::u32:
            oss_ << ent.value().as<uint32_t>();
            return;
        case builtin_eid::i64:
            oss_ << ent.value().as<int64_t>();
            return;
        case builtin_eid::u64:
            oss_ << ent.value().as<uint64_t>();
            return;
        case builtin_eid::f16:
            oss_ << ent.value().as<float16>();
            return;
        case builtin_eid::f32:
            oss_ << ent.value().as<float>();
            return;
        case builtin_eid::f64:
            oss_ << ent.value().as<double>();
            return;

        default:
            THROW_INTERNAL_ERROR("to_string_visitor: Unsupported generic literal type for to_string");
        }
    }
    
    void operator()(const identifier_entity& ent) const override
    {
        u_.print_to(oss_, ent.value());
    }
    
    void operator()(const qname_identifier_entity& ent) const override
    {
        u_.print_to(oss_, ent.value());
    }
    
    void operator()(const empty_entity& e) const override
    {
        e.print_to(oss_, u_);
    }
    
    // Other entity types
    void operator()(const function_entity& ent) const override
    {
        ent.print_to(oss_, u_);
    }
    
    void operator()(const external_function_entity& ent) const override
    {
        ent.print_to(oss_, u_);
    }
    
    void operator()(const extern_variable_entity& ent) const override
    {
        ent.print_to(oss_, u_);
    }
    
    //void operator()(const functional_entity& ent) const override
    //{
    //    ent.print_to(oss_, u_);
    //}
};

std::expected<syntax_expression_result_t, error_storage> to_string_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto & [_, er] = md.matches.front();
    
    er.expressions = el.concat(md.merge_void_spans(el), er.expressions);
    if (er.is_const_result) {
        entity const& ent = get_entity(u, er.value());
        std::ostringstream oss;
        ent.visit(to_string_visitor(u, oss));
        er.value_or_type = u.make_string_entity(oss.str()).id;
    } else {
        u.push_back_expression(el, er.expressions, semantic::invoke_function(u.get(builtin_eid::to_string)));
        er.value_or_type = u.get(builtin_eid::string);
    }
    return std::move(er);
}

}
