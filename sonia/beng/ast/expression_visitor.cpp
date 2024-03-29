//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "expression_visitor.hpp"
#include "expression_cast_visitor.hpp"
#include "expression_decimal_visitor.hpp"
#include "expression_vector_visitor.hpp"
#include "expression_fn_visitor.hpp"
#include "expression_type_visitor.hpp"

#include "sonia/utility/scope_exit.hpp"

#include "fn_compiler_context.hpp"
#include "../entities/enum_entity.hpp"
#include "../entities/functional_entity.hpp"

namespace sonia::lang::beng {

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

expression_visitor::result_type expression_visitor::operator()(variable_identifier const& var) const
{
    shared_ptr<entity> e = ctx.resolve_entity(var.name);
    if (auto varptr = dynamic_cast<variable_entity*>(e.get()); varptr) {
        variable_entity::kind k = varptr->varkind();
        if (k == variable_entity::kind::EXTERN || k == variable_entity::kind::STATIC) {
            THROW_NOT_IMPLEMENTED_ERROR();
        }
        if (k == variable_entity::kind::SCOPE_LOCAL || k == variable_entity::kind::LOCAL) {
            if (!varptr->name().parent().has_prefix(ctx.base_ns())) {
                if (k == variable_entity::kind::SCOPE_LOCAL || var.scope_local) {
                    throw exception("%1%(%2%,%3%): `%4%`: variable is not defined in the scope"_fmt %
                        var.location.resource % var.location.line % var.location.column %
                        ctx.u().print(var.name));
                }
                varptr = &ctx.create_captured_variable_chain(*varptr);
            }
        } else {
            THROW_INTERNAL_ERROR("unknown variable kind");
        }

        result.emplace_back(semantic::push_variable{ varptr });
        if (!expected_result || *expected_result == varptr->type()) {
            return varptr->type();
        }
        expression_cast_visitor etvis{ ctx, varptr->type(), result };
        if (auto optrest = apply_visitor(etvis, *expected_result); optrest) {
            return *optrest;
        }
        /*
        expression_type_visitor etvis{ ctx, *expected_result, result };
        if (auto optrest = apply_visitor(etvis, varptr->type()); optrest) {
            return *optrest;
        }
        */
        throw exception("%1%(%2%,%3%): `%4%`: cannot convert from `%5%` to `%6%`"_fmt %
            var.location.resource % var.location.line % var.location.column %
            ctx.u().print(var.name) % ctx.u().print(varptr->type()) % ctx.u().print(*expected_result));
        // type to type converter
    } else if (auto fnptr = dynamic_pointer_cast<functional_entity>(e); fnptr) {
        if (!expected_result) {
            THROW_NOT_IMPLEMENTED_ERROR("forward functional template");
        }
        expression_fn_visitor dvis{ ctx, *fnptr, result };
        if (auto optrest = apply_visitor(dvis, *expected_result); optrest) {
            return *optrest;
        }
        return nullopt;
    }
    /*
    // to do: look for variable first
    if (variable_entity const* pv = ctx.resolve_variable(aqnm.name); pv) {
        result.emplace_back(semantic::push_variable{ pv });
        return pv->type();
    }
    */
    ctx.throw_undeclared_identifier(var.name, var.location);
}

expression_visitor::result_type expression_visitor::operator()(binary_expression_t<operator_type::ASSIGN> const& op) const
{
    expression_visitor evis{ ctx, result, nullptr };
    auto rtype = apply_visitor(evis, op.right);

    if (variable_identifier const* varnm = get<variable_identifier>(&op.left); varnm) {
        auto optvar = ctx.resolve_variable(varnm->name);
        if (!optvar) {
            ctx.throw_undeclared_identifier(varnm->name, varnm->location);
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

    variable_identifier const* fnvar = get<variable_identifier>(&proc.fn_object);
    if (!fnvar) {
        THROW_NOT_IMPLEMENTED_ERROR("fn object expressions is not implemented yet");
    }
    shared_ptr<entity> e = ctx.resolve_entity(fnvar->name);
    if (!e) [[unlikely]] {
        ctx.throw_undeclared_identifier(fnvar->name, fnvar->location);
    }
    shared_ptr<functional_entity> func_ent = dynamic_pointer_cast<functional_entity>(e);
    if (!func_ent) [[unlikely]] {
        // to do: can be variable
        throw exception("name '%1%' is not callable"_fmt % ctx.u().print(fnvar->name));
    }
    beng_type rtype;
    if (!func_ent->find(ctx, proc.positioned_args, proc.named_args, result, rtype)) {
        throw exception("can't match a function call '%1%'"_fmt % ctx.u().print(fnvar->name));
    }

    return rtype;
}

}
