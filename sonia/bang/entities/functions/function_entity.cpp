//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "function_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/preliminary_type_visitor.hpp"
#include "sonia/bang/ast/declaration_visitor.hpp"

namespace sonia::lang::bang {

//size_t function_entity::hash() const noexcept
//{
//    size_t hv = hash_value(name_);
//    for (fieldset_t::positioned_field const& f : fs_->positioned_fields()) {
//        hash_combine(hv, f.constraint);
//    }
//    for (fieldset_t::named_field const& f : fs_->named_fields()) {
//        hash_combine(hv, f.ename.value);
//        hash_combine(hv, f.constraint);
//    }
//    return hv;
//}
//
//bool function_entity::equal(entity const& rhs) const noexcept
//{
//    if (function_entity const* pfe = dynamic_cast<function_entity const*>(&rhs); pfe) {
//        if (pfe->name_ != name_) return false;
//        if (!std::ranges::equal(pfe->fs_->positioned_fields(), fs_->positioned_fields(),
//            [](fieldset_t::positioned_field const& l, fieldset_t::positioned_field const& r) {
//                return l.constraint == r.constraint &&
//                       l.constraint_type == r.constraint_type;
//            })) return false;
//        return std::ranges::equal(pfe->fs_->named_fields(), fs_->named_fields(),
//            [](fieldset_t::named_field const& l, fieldset_t::named_field const& r) {
//                return l.ename.value == r.ename.value &&
//                       l.constraint == r.constraint &&
//                       l.constraint_type == r.constraint_type;
//            });
//    }
//    return false;
//}

size_t function_entity::parameter_count() const noexcept
{
    // to do: include captured parameters
    size_t cnt = 0;
    for (fieldset_t::positioned_field const& f : fs_->positioned_fields()) {
        if (f.constraint_type == parameter_constraint_modifier_t::value_type_constraint) ++cnt;
    }
    for (fieldset_t::named_field const& f : fs_->named_fields()) {
        if (f.constraint_type == parameter_constraint_modifier_t::value_type_constraint) ++cnt;
    }
    return cnt;
}

void function_entity::set_fn_signature(unit& u, entity_signature&& fnsig)
{
    indirect_signatured_entity te{ fnsig };
    entity const& fnte = u.eregistry().find_or_create(te, [&fnsig, &u]() {
        return make_shared<basic_signatured_entity>(u.get_typename_entity_identifier(), std::move(fnsig));
    });
    field_descriptor const* fd = fnsig.find_field(u.get_fn_result_identifier());
    is_void_ = !fd;
    result_type_ = is_void_ ? u.get_void_entity_identifier() : fd->entity_id();
}

//void function_entity::set_fn_type(unit& u, entity_signature& fnsig)
//{
//    fnsig.push(u.get_fn_result_identifier(), {result_type_, false});
//    fnsig.normilize();
//
//    // resolve function type entity
//    indirect_signatured_entity te{fnsig};
//    entity const& fnte = u.eregistry().find_or_create(te, [&te, &u]() {
//        te.set_type(u.get_typename_entity_identifier());
//        return make_shared<type_entity>(std::move(te));
//    });
//    set_type(fnte.id());
//    is_void_ = result_type_ == u.get_void_entity_identifier();
//}

void function_entity::build(unit& u)
{
    BOOST_ASSERT(!is_built_);

    THROW_NOT_IMPLEMENTED_ERROR("function_entity::build");
#if 0
    qname_view fn_qname = u.fregistry().resolve(name_).name();
    fn_compiler_context fnctx{ u, fn_qname / u.new_identifier() };

    // setup parameters & signature
    entity_signature fnsig{ u.get_fn_qname_identifier() };
    boost::container::small_vector<variable_entity*, 16> params;
    std::array<char, 16> argname = { '$' };
    size_t argindex = 0;
    for (auto const& f : fs_->positioned_fields()) {
        if (f.constraint_type == parameter_constraint_modifier_t::value_constraint) {
            THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor fn_decl_t with value constraints");
        }
        else if (f.constraint_type == parameter_constraint_modifier_t::typename_constraint) {
            THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor fn_decl_t with typename_constraint constraints");
        }
        BOOST_ASSERT(f.constraint_type == parameter_constraint_modifier_t::value_type_constraint);
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
        qname_identifier param_qname_id = u.fregistry().resolve(fnctx.ns() / fname).id();
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

    declaration_visitor dvis{ fnctx };
    for (infunction_declaration_t& d : decl_body_) {
        apply_visitor(dvis, d);
    }
    fnctx.finish_frame();
    BOOST_ASSERT(fnctx.expressions_branch() == 1);
    body_ = std::move(fnctx.expressions());

    result_type_ = fnctx.compute_result_type();
    set_fn_type(u, fnsig);

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
    is_built_ = true;
}

}
