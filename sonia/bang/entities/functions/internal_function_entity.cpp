//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "internal_function_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/declaration_visitor.hpp"

namespace sonia::lang::bang {

internal_function_entity::internal_function_entity(qname&& name, entity_signature&& sig, statement_span sts)
    : function_entity{ std::move(name), std::move(sig) }
    , sts_{ std::move(sts) }
    , is_inline_{ 0 }
    , is_built_{ 0 }
{
}

void internal_function_entity::push_argument(annotated_identifier name, local_variable&& v)
{
    push_variable(v.varid, arg_count_);
    bound_arguments.emplace_back(std::move(name), std::move(v));
    ++arg_count_;
}

void internal_function_entity::push_variable(variable_identifier varid, intptr_t index)
{
    BOOST_VERIFY(variables_.insert(std::pair{ varid, index }).second);
}

//void internal_function_entity::push_argument(annotated_identifier name, local_variable&& lv)
//{
//    variables_.insert(lv.varid, std::move(lv));
//    variables_.emplace_back(lv.varid);
//    bound_arguments.emplace_back(name, std::move(lv));
//}
//
//void internal_function_entity::push_variable(local_variable&& lv)
//{
//    variables_.emplace_back(lv.varid);
//}

intptr_t internal_function_entity::resolve_variable_index(variable_identifier varid) const
{
    auto it = variables_.find(varid);
    if (it == variables_.end()) {
        THROW_INTERNAL_ERROR("internal_function_entity::resolve_variable_index (%1%) variable not found"_fmt % varid);
    }
    return it->second - arg_count_;
    //auto it = variables_.get<1>().find(varid);
    //if (it == variables_.get<1>().end()) {
    //    THROW_INTERNAL_ERROR("internal_function_entity::resolve_variable_index (%1%) variable not found"_fmt % varid);
    //}
    //auto pos = variables_.project<0>(it) - variables_.get<0>().begin();
    //return static_cast<intptr_t>(bound_arguments.size()) - static_cast<intptr_t>(pos) - 1;
}

void internal_function_entity::build(unit& u)
{
    fn_compiler_context fnctx{ u, name_ };
    fnctx.push_binding(bound_arguments);
    build(fnctx);
}

void internal_function_entity::build(fn_compiler_context& fnctx)
{
    BOOST_ASSERT(!is_built_);

    if (result.entity_id()) {
        fnctx.result_value_or_type = result.entity_id();
        fnctx.is_const_result = result.is_const();
    }

    declaration_visitor dvis{ fnctx, *this };
    if (auto err = dvis.apply(sts_); err) throw exception(fnctx.u().print(*err));

    auto [value_or_type, is_value] = fnctx.finish_frame(); // unknown result type is resolving here

    if (!result.entity_id()) {
        result = field_descriptor{ value_or_type, is_value };
    }
    
    BOOST_ASSERT(fnctx.expressions_branch() == 1);
    body = fnctx.expressions();
    fnctx.u().store(std::move(fnctx.expression_store()));

    BOOST_ASSERT(!fnctx.expression_store());

    //GLOBAL_LOG_INFO() << "built inline function begin: " << u.print(*this);
    //body.for_each([&u](semantic::expression const& e) {
    //    GLOBAL_LOG_INFO() << u.print(e);
    //});
    //GLOBAL_LOG_INFO() << "built inline function end: " << u.print(*this);
    //sts_.reset();
    is_built_ = 1;
}

}
