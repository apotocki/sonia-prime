//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "struct_get_pattern.hpp"
#include "struct_entity.hpp"

#include "sonia/bang/entities/tuple/generic_get_pattern_matcher.ipp"

#include "sonia/bang/entities/signatured_entity.hpp"

//#include "sonia/bang/ast/base_expression_visitor.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

//class struct_get_match_descriptor : public functional_match_descriptor
//{
//    size_t property_index_;
//    size_t fields_count_;
//
//public:
//    using functional_match_descriptor::functional_match_descriptor;
//
//    inline size_t property_index() const noexcept { return property_index_; }
//    inline size_t fields_count() const noexcept { return fields_count_; }
//
//    inline void set_property_index(size_t index) noexcept { property_index_ = index; }
//    inline void set_fields_count(size_t index) noexcept { fields_count_ = index; }
//};

/**
    inline fn::get(self: $T @structure, property: const __identifier @has_property($T)) =>
        get(self: as_tuple(self), property : property);
*/

class struct_get_match_descriptor : public functional_match_descriptor
{
public:
    inline struct_get_match_descriptor(prepared_call const& call, entity const& ent) noexcept
        : functional_match_descriptor{ call }
        , tpl_entity{ ent }
    { }

    entity const& tpl_entity;
};

struct stucture_get_pattern_matcher : public generic_get_pattern_matcher<stucture_get_pattern_matcher>
{
    using generic_get_pattern_matcher<stucture_get_pattern_matcher>::generic_get_pattern_matcher;
    struct_entity const* pse = nullptr;

    inline error_storage check_type_compatibility()
    {
        entity const& some_entity = get_entity(ctx.u(), slftype);
        pse = dynamic_cast<struct_entity const*>(&some_entity);
        if (!pse) {
            return make_error<basic_general_error>(get_start_location(*get<0>(slf_arg_descr)), "argument type mismatch: a structure was expected."sv, slftype);
        }
        return {};
    }

    std::expected<functional_match_descriptor_ptr, error_storage> create_match_descriptor()
    {
        unit& u = ctx.u();
        auto uteid = pse->underlying_tuple_eid(ctx);
        if (!uteid) return std::unexpected(append_cause(
            make_error<basic_general_error>(call.location, "cannot get underlying tuple entity for structure"sv, pse->id),
            std::move(uteid.error())
        ));
        return make_shared<struct_get_match_descriptor>(call, get_entity(u, *uteid));
    }
};

std::expected<functional_match_descriptor_ptr, error_storage> struct_get_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    stucture_get_pattern_matcher matcher{ ctx, call, exp };
    return matcher.try_match();
        
#if 0
    //return check_match(std::move(pmd), call, utplent, *ppname);

    //entity_signature const* ptplsig = utplent.signature();
    //BOOST_ASSERT(ptplsig);

    //size_t index;
    //field_descriptor const* fd;
    //if (identifier_entity const* pie = dynamic_cast<identifier_entity const*>(ppname); pie) {
    //    fd = ptplsig->find_field(pie->value(), &index);
    //} else if (integer_literal_entity const* pile = dynamic_cast<integer_literal_entity const*>(ppname); pile) {
    //    fd = ptplsig->find_field((size_t)pile->value(), &index);
    //} else {
    //    return std::unexpected(make_error<basic_general_error>(ppname->location(), "argument mismatch, expected an identifier or index"sv, ppname->id()));
    //}
    //
    //if (!fd) {
    //    return std::unexpected(make_error<basic_general_error>(call.location(), "no such field"sv, ppname->id()));
    //}

    //pmd->set_property_index(index);
    //pmd->set_fields_count(ptplsig->field_count());
    //BOOST_ASSERT(ptplsig->field_count() > 1 || !index);

    //pmd->result = *fd;
    //return pmd;
#endif
}

std::expected<syntax_expression_result_t, error_storage> struct_get_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<struct_get_match_descriptor&>(md);
    auto& slfer = get<1>(md.matches[0]);
    auto& proper = get<1>(md.matches[1]);

    local_variable* tuple_var = nullptr, *property_var = nullptr;
    identifier tuple_var_name, property_var_name;
    fn_compiler_context_scope fn_scope{ ctx };

    pure_call_t get_call{ md.call_location };
    if (slfer.is_const_result) {
        // if self is a constant, then we can use it directly
        get_call.emplace_back(
            annotated_identifier{ u.get(builtin_id::self), md.call_location },
            annotated_entity_identifier{ slfer.value(), md.call_location });
    } else {
        tuple_var_name = u.new_identifier();
        tuple_var = &fn_scope.new_temporary(tuple_var_name, tmd.tpl_entity.id); // here we substitute the tuple entity id instead of the original struct type id
        get_call.emplace_back(
            annotated_identifier{ u.get(builtin_id::self), md.call_location },
            variable_reference{ annotated_qname{ qname{ tuple_var_name, false } }, false });
    }
    if (proper.is_const_result) {
        // if property is a constant, then we can use it directly
        get_call.emplace_back(
            annotated_identifier{ u.get(builtin_id::property) },
            annotated_entity_identifier{ proper.value(), md.call_location });
    } else {
        property_var_name = u.new_identifier();
        property_var = &fn_scope.new_temporary(property_var_name, proper.type());
        get_call.emplace_back(
            annotated_identifier{ u.get(builtin_id::property), md.call_location },
            variable_reference{ annotated_qname{ qname{ property_var_name, false } }, false });
    }

    auto match = ctx.find(builtin_qnid::get, get_call, el);
    if (!match) {
        return std::unexpected(append_cause(
            make_error<basic_general_error>(md.call_location, "cannot find get function for underlying structure tuple"sv, tmd.tpl_entity.id),
            std::move(match.error())
        ));
    }
    auto res = match->apply(ctx);
    if (!res) {
        return std::unexpected(std::move(res.error()));
    }

    syntax_expression_result_t result{
        .temporaries = std::move(slfer.temporaries),
        .branches_expressions = el.concat(slfer.branches_expressions, proper.branches_expressions),
        .expressions = res->expressions,
        .value_or_type = res->value_or_type,
        .is_const_result = res->is_const_result
    };
    if (!proper.temporaries.empty()) {
        result.temporaries.insert(
            result.temporaries.end(),
            std::make_move_iterator(proper.temporaries.begin()),
            std::make_move_iterator(proper.temporaries.end()));
    }

    if (tuple_var) {
        result.temporaries.emplace_back(tuple_var_name, std::move(*tuple_var), slfer.expressions);
    }
    if (property_var) {
        result.temporaries.emplace_back(property_var_name, std::move(*property_var), proper.expressions);
    }

    return result;
}

//error_storage struct_get_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
//{
//    unit& u = ctx.u();
//
//    // push call expressions in the right order
//    semantic::expression_list_t& exprs = ctx.expressions();
//
//    // only one named argument is expected
//    md.for_each_named_match([&exprs, &md](identifier name, parameter_match_result const& mr) {
//        for (auto rng : mr.expressions) {
//            ++rng.second;
//            exprs.splice_back(md.call_expressions, rng.first, rng.second);
//        }
//    });
//
//    BOOST_ASSERT(!md.call_expressions); // all arguments were transfered
//    
//    struct_get_match_descriptor& smd = static_cast<struct_get_match_descriptor&>(md);
//    if (smd.fields_count() > 1) {
//        u.push_back_expression(exprs, semantic::push_value{ smd.property_index() });
//        u.push_back_expression(exprs, semantic::invoke_function(u.get(builtin_eid::array_at)));
//    }
//    
//    ctx.context_type = md.result.entity_id();
//    return {};
//}

} // namespace sonia::lang::bang
