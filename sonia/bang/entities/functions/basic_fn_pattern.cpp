//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "basic_fn_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/preliminary_type_visitor.hpp"
#include "sonia/bang/ast/declaration_visitor.hpp"

//#include "sonia/bang/entities/ellipsis/pack_entity.hpp"
#include "sonia/bang/entities/functional_entity.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"

namespace sonia::lang::bang {

basic_fn_pattern::basic_fn_pattern(fn_compiler_context& ctx, fn_decl_t& fnd)
    : fs_{ make_shared<fieldset>() }
{
    unit& u = ctx.u();
    declaration_location_ = fnd.location();
    qname fn_qname = ctx.ns() / fnd.name();
    fn_qname_id_ = u.qnregistry().resolve(fn_qname);

    if (auto err = ctx.build_fieldset(fnd, *fs_); err) {
        throw exception(u.print(*err));
    }

    // now we know if it's a template or a general function
    // to do: implement templates
    
    // if a result type is declared it should be resolved in the declaration context => shouldn't be postponed
    if (fnd.result) {
        preliminary_type_visitor2 tqvis{ ctx };
        result_type_ = apply_visitor(tqvis, *fnd.result);

        entity_signature fnsig{ u.get_fn_qname_identifier() };
        for (auto const& f : fs_->positioned_fields()) {
            if (f.constraint_type == param_constraint_type::type_constaint) {
                fnsig.push(f.constraint);
            }
        }
        for (auto const& f : fs_->named_fields()) {
            if (f.constraint_type == param_constraint_type::type_constaint) {
                fnsig.push(f.ename.value, f.constraint);
            }
        }
        // create function entity
        auto fn_entity = make_shared<function_entity>(fn_qname_id_, fs_, result_type_);
        fn_entity->set_fn_type(u, fnsig);
        
        u.eregistry().insert(fn_entity); //
        fn_entity->set_body(std::move(fnd.body));
        fn_entity_id_ = fn_entity->id();

        fn_entity->set_const_index(u.allocate_constant_index());
    } else {
        THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern without explicit result type");
    }
}

std::expected<entity_identifier, error_storage> basic_fn_pattern::apply(fn_compiler_context& ctx) const
{
    if (!result_type_) { // the result type should be inferred by the function content
        if (building_) {
            throw circular_dependency_error({make_error<basic_general_error>(declaration_location_, "resolving function result type"sv, fn_qname_id_)});
        }
        const_cast<basic_fn_pattern*>(this)->build(ctx.u());
    }
    BOOST_ASSERT(fn_entity_id_);
    ctx.expressions().push_back(semantic::invoke_function(fn_entity_id_));
    return result_type_;
    //THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern::apply");
}

void basic_fn_pattern::build(unit& u)
{
    BOOST_ASSERT(!building_);
    building_ = true;
    function_entity& fe = const_cast<function_entity&>(dynamic_cast<function_entity const&>(u.eregistry().get(fn_entity_id_)));
    fe.build(u);
    result_type_ = fe.get_result_type();
    building_ = false;
#if 0
    qname_view fn_qname = u.qnregistry().resolve(fn_qname_id_);
    fn_compiler_context fnctx{ u, fn_qname / u.new_identifier() };

    // setup parameters & signature
    entity_signature fnsig{ u.get_fn_qname_identifier() };
    boost::container::small_vector<variable_entity*, 16> params;
    std::array<char, 16> argname = { '$' };
    size_t argindex = 0;
    for (auto const& f : fs_->positioned_fields()) {
        if (f.constraint_type == param_constraint_type::value_constaint) {
            THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor fn_decl_t with value constraints");
        }
        else if (f.constraint_type == param_constraint_type::const_constraint) {
            THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor fn_decl_t with const constraints");
        }
        BOOST_ASSERT(f.constraint_type == param_constraint_type::type_constaint);
        fnsig.push(f.constraint);
        identifier fname;
        if (f.iname) {
            fname = f.iname->value;
        } else {
            bool reversed = false;
            char* epos = mp::to_string(span{ &argindex, 1 }, argname.data() + 1, reversed);
            if (reversed) std::reverse(argname.data() + 1, epos);
            fname = u.slregistry().resolve(string_view{ argname.data(), epos });
        }
        qname_identifier param_qname_id = u.qnregistry().resolve(fnctx.ns() / fname);
        ++argindex;

        auto var_entity = make_shared<variable_entity>(f.constraint, param_qname_id, variable_entity::kind::SCOPE_LOCAL);
        u.eregistry().insert(var_entity);
        functional& param_fnl = u.fregistry().resolve(param_qname_id);
        param_fnl.set_default_entity(var_entity->id());
        params.emplace_back(var_entity.get());
    }
    for (auto const& f : fs_->named_fields()) {
        fnsig.push(f.ename.value, f.constraint);
        THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor fn_decl_t with named fields");
    }

    function_entity & fe = const_cast<function_entity&>(dynamic_cast<function_entity const&>(u.eregistry().get(fn_entity_id_)));

    declaration_visitor dvis{ fnctx };
    for (infunction_declaration_t& d : fe.body()) {
        apply_visitor(dvis, d);
    }
    fnctx.finish_frame();

    result_type_ = fnctx.result;
    entity_identifier fntypeid = build_fn_type(u, fnsig);
    fe.set_type(fntypeid);

    intptr_t paramnum = 0;
    size_t paramcount = params.size() + fnctx.captured_variables.size();
    for (variable_entity* var : params) {
        var->set_index(paramnum - paramcount);
        ++paramnum;
    }
    for (auto [from, tovar] : fnctx.captured_variables) {
        tovar->set_index(paramnum - paramcount);
        ++paramnum;
    }
#endif
}

}
