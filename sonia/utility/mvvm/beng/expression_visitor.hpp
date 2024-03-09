//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "beng.hpp"
#include "unit.hpp"
#include "ast.hpp"

#include "semantic.hpp"
#include "sonia/optional.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia::lang::beng {

struct expression_visitor : static_visitor<optional<beng_generic_type>>
{
    compiler_context& ctx;
    beng_generic_type const* expected_result;
    std::vector<semantic_expression_type>& result;

    explicit expression_visitor(compiler_context& c, std::vector<semantic_expression_type>& r, beng_generic_type const* er)
        : ctx{ c }
        , result { r }
        , expected_result{ er }
    {}

    result_type operator()(qname const&) const;

    result_type operator()(case_expression const&) const;

    result_type operator()(decimal const&) const;
    
    result_type operator()(small_u32string const&) const;
    
    result_type operator()(expression_vector_t const&) const;

    result_type operator()(function_call_t const&) const;

    result_type operator()(binary_expression_t<operator_type::ASSIGN> const&) const;
};

}

#include "compiler.hpp"
#include "enum_entity.hpp"
#include "functional_entity.hpp"

namespace sonia::lang::beng {

struct expression_decimal_visitor : static_visitor<optional<beng_generic_type>>
{
    compiler_context& ctx;
    decimal const& dec;
    std::vector<semantic_expression_type>& result;

    expression_decimal_visitor(compiler_context& c, decimal const& d, std::vector<semantic_expression_type>& r)
        : ctx{ c }
        , dec{ d }
        , result{ r }
    {}

    inline result_type operator()(beng_decimal_t const&) const
    {
        result.emplace_back(semantic::push_value{ dec });
        return beng_decimal_t{};
    }

    inline result_type operator()(beng_int_t const&) const
    {
        result.emplace_back(semantic::push_value{ dec });
        return beng_int_t{};
    }

    inline result_type operator()(beng_float_t const&) const
    {
        result.emplace_back(semantic::push_value{ dec });
        return beng_float_t{};
    }

    result_type operator()(beng_union_t const& v) const
    {
        for (beng_generic_type const& ut : v.members) {
            if (auto optrest = apply_visitor(*this, ut); optrest) {
                return *optrest;
            }
        }
        return nullopt;
    }

    inline result_type operator()(beng_bool_t const& v) const { return nullopt; }
    inline result_type operator()(beng_string_t const& v) const { return nullopt; }
    inline result_type operator()(beng_object_t const& v) const { return nullopt; }
    inline result_type operator()(beng_array_t const& v) const { return nullopt; }
    inline result_type operator()(beng_vector_t const& v) const { return nullopt; }
    inline result_type operator()(beng_tuple_t const& v) const { return nullopt; }
    inline result_type operator()(beng_fn_t const& v) const { return nullopt; }
    /*
    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    */
};

expression_visitor::result_type expression_visitor::operator()(decimal const& d) const
{
    if (!expected_result) {
        result.emplace_back(semantic::push_value{ d });
        return beng_decimal_t{};
    }
    expression_decimal_visitor dvis{ ctx, d, result };
    if (auto optrest = apply_visitor(dvis, *expected_result); optrest) {
        return *optrest;
    }
    return nullopt;
    /*
    throw exception("decimal '%1%' is not an expected type %2%"_fmt % d % ctx.u().print(*expected_result));
    */
}

expression_visitor::result_type expression_visitor::operator()(small_u32string const& e) const
{
    result.emplace_back(semantic::push_value{ e });
    if (!expected_result || get<beng_string_t>(expected_result)) return beng_string_t{};
    result.pop_back();
    namespace cvt = boost::conversion;
    boost::container::small_vector<char, 32> result;
    (cvt::cvt_push_iterator(cvt::utf32 | cvt::utf8, std::back_inserter(result)) << e).flush();
    throw exception("string '%1%' is not an expected type %2%"_fmt % string_view{ result.data(), result.size() } % ctx.u().print(*expected_result));
}

expression_visitor::result_type expression_visitor::operator()(qname const& e) const
{
    // to do: look for variable first
    if (variable_entity const* plv = ctx.resolve_variable(e); plv) {
        result.emplace_back(semantic::push_variable{ plv });
        return plv->type();
    }
    THROW_NOT_IMPLEMENTED_ERROR();
}

expression_visitor::result_type expression_visitor::operator()(binary_expression_t<operator_type::ASSIGN> const& op) const
{
    expression_visitor evis{ ctx, result, nullptr };
    auto rtype = apply_visitor(evis, op.right);

    if (qname const* varnm = get<qname>(&op.left); varnm) {
        auto optvar = ctx.resolve_variable(*varnm);
        if (!optvar) {
            throw exception("variable '%1%' is not defined"_fmt % ctx.u().print(*varnm));
        }
        result.emplace_back(semantic::set_variable{ optvar });
        return std::move(rtype);
    }
    THROW_NOT_IMPLEMENTED_ERROR();
}

expression_visitor::result_type expression_visitor::operator()(case_expression const& ce) const
{
    if (!expected_result) {
        throw exception("no context type to resolve the case expression %1%"_fmt % ctx.u().print(ce.name));
    }
    beng_object_t const* pobj = get<beng_object_t>(expected_result);
    if (!pobj) {
        throw exception("the context type isn't an object to resolve the case expression %1%"_fmt % ctx.u().print(ce.name));
    }
    shared_ptr<entity> e = ctx.resolve_entity(pobj->name());
    if (!e) [[unlikely]] {
        throw exception("unresolved context object '%1%'"_fmt % ctx.u().print(pobj->name()));
    }
    shared_ptr<enum_entity> enum_ent = dynamic_pointer_cast<enum_entity>(e);
    if (!enum_ent) [[unlikely]] {
        throw exception("name '%1%' is not a enumerration"_fmt % ctx.u().print(pobj->name()));
    }
    auto const* enumcase = enum_ent->find(ce.name);
    if (!enumcase) [[unlikely]] {
        throw exception("name '%1%' is not a case of the enumerration %2%"_fmt % ctx.u().print(ce.name) % ctx.u().print(pobj->name()));
    }
    result.emplace_back(semantic::push_value{ enumcase->value });
    return *expected_result;
}

struct expression_vector_visitor : static_visitor<optional<beng_generic_type>>
{
    compiler_context& ctx;
    expression_vector_t const& vec;
    std::vector<semantic_expression_type>& result;

    expression_vector_visitor(compiler_context& c, expression_vector_t const& v, std::vector<semantic_expression_type>& r)
        : ctx{ c }
        , vec { v }
        , result{ r }
    {}

    result_type operator()(beng_vector_t const& v) const
    {
        size_t stored_pos = result.size();
        EXCEPTIONAL_SCOPE_EXIT([this, stored_pos](){ result.resize(stored_pos); });
        expression_visitor elemvis{ ctx, result, &v.type };
        for (expression_t const& e : vec.elements) {
            if (!apply_visitor(elemvis, e)) {
                result.resize(stored_pos);
                return nullopt;
            }
        }
        result.emplace_back(semantic::push_value{ decimal{ vec.elements.size() } });
        result.emplace_back(semantic::invoke_function{ ctx.u().arrayify_entity_name(), (uint32_t)vec.elements.size() + 1 });
        return v;
    }

    result_type operator()(beng_array_t const& ar) const
    {
        if (ar.size != vec.elements.size() || !this->operator()(beng_vector_t{ ar.type })) return nullopt;
        return ar;
    }

    result_type operator()(beng_union_t const& v) const
    {
        for (beng_generic_type const& ut : v.members) {
            if (auto optrest = apply_visitor(*this, ut); optrest) {
                return *optrest;
            }
        }
        return nullopt;
    }

    inline result_type operator()(beng_bool_t const& v) const { return nullopt; }
    inline result_type operator()(beng_int_t const& v) const { return nullopt; }
    inline result_type operator()(beng_float_t const& v) const { return nullopt; }
    inline result_type operator()(beng_decimal_t const& v) const { return nullopt; }
    inline result_type operator()(beng_string_t const& v) const { return nullopt; }
    inline result_type operator()(beng_object_t const& v) const { return nullopt; }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

expression_visitor::result_type expression_visitor::operator()(expression_vector_t const& vec) const
{
    if (!expected_result) { // is real case?
        beng_tuple_t rtype;
        rtype.fields.reserve(vec.elements.size());
        expression_visitor elemvis{ ctx, result, nullptr };
        for (expression_t const& e : vec.elements) {
            rtype.fields.emplace_back(*apply_visitor(elemvis, e));
        }
        return std::move(rtype);
    }

    expression_vector_visitor evv{ ctx, vec, result };
    if (auto optrest = apply_visitor(evv, *expected_result); optrest) {
        return *optrest;
    }
    
    beng_tuple_t rtype;
    rtype.fields.reserve(vec.elements.size());
    expression_visitor elemvis{ ctx, result, nullptr };
    for (expression_t const& e : vec.elements) {
        rtype.fields.emplace_back(*apply_visitor(elemvis, e));
    }
    throw exception("can't convert the %1% to %2%"_fmt % ctx.u().print(rtype) % ctx.u().print(*expected_result));
    /*
    beng_generic_type const* expected_elem_type = nullptr;
    if (expected_result) {
        if (beng_vector_t const* parr = get<beng_vector_t>(expected_result); parr) {
            expected_elem_type = &parr->type;

            expression_visitor elemvis{ ctx, result, expected_elem_type };
            for (expression_t const& e : vec.elements) {
                apply_visitor(elemvis, e);
            }
            result.emplace_back(semantic::push_value{ decimal{ vec.elements.size() } });
            result.emplace_back(semantic::invoke_function{ &ctx.u().arrayify_entity(), (uint32_t)vec.elements.size() + 1 });
            return *expected_result;
        }
        THROW_NOT_IMPLEMENTED_ERROR("expected type: %1%"_fmt % ctx.u().print(*expected_result));
    }

    
    return std::move(rtype);
    */
}

expression_visitor::result_type expression_visitor::operator()(function_call_t const& proc) const
{
    //expression_visitor evis{ ctx, nullptr };
    //semantic_expression_pair epair = apply_visitor(evis, proc.subject);
    
    // check uniqueness
    if (auto it = std::ranges::adjacent_find(proc.named_args, {}, [](auto const& pair) { return std::get<0>(pair).id; }); it != proc.named_args.end()) {
        ++it; // get second
        auto const& aid = std::get<0>(*it);
        throw exception("%1%(%2%,%3%): duplicate argument `%4%`"_fmt % aid.location.resource % aid.location.line % aid.location.column % ctx.u().print(aid.id));
    }

    shared_ptr<entity> e = ctx.resolve_entity(proc.name);
    if (!e) [[unlikely]] {
        throw exception("unresolved function name '%1%'"_fmt % ctx.u().print(proc.name));
    }
    shared_ptr<functional_entity> func_ent = dynamic_pointer_cast<functional_entity>(e);
    if (!func_ent) [[unlikely]] {
        throw exception("name '%1%' is not callable"_fmt % ctx.u().print(proc.name));
    }
    beng_generic_type rtype;
    if (!func_ent->find(ctx, proc.positioned_args, proc.named_args, result, rtype)) {
        throw exception("can't match a function call '%1%'"_fmt % ctx.u().print(proc.name));
    }

    return rtype;
}

}
