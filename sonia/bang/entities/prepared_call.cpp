//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include "prepared_call.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/errors.hpp"

#include "sonia/bang/errors/no_position_argument_error.hpp"

namespace sonia::lang::bang {

prepared_call::prepared_call(fn_compiler_context& ctx, semantic::expression_list_t& ael, lex::resource_location loc) noexcept
    : caller_ctx{ ctx }
    , expressions{ ael }
    , location{ std::move(loc) }
{
    ctx.push_binding(bound_temporaries);
}

prepared_call::prepared_call(fn_compiler_context &ctx, pure_call_t const& call, semantic::expression_list_t& ael)
    : caller_ctx{ ctx }
    , expressions{ ael }
    , location{ call.location }
    , args{ call.args }
{
    // initialize caches
    for (auto const& arg : args) {
        auto [name, expr] = *arg;
        if (name) {
            named_argument_caches_.emplace_back(name->value, argument_cache{ expr });
        } else {
            position_argument_caches_.emplace_back(expr);
        }
    }
    // sort named arguments
    std::ranges::sort(named_argument_caches_, {}, [](auto const& pair) { return get<0>(pair).value; });
    ctx.push_binding(bound_temporaries);
}

prepared_call::~prepared_call()
{
    caller_ctx.pop_binding(&bound_temporaries);
}

local_variable& prepared_call::new_temporary(unit& u, identifier name, entity_identifier type, semantic::expression_span el)
{
    local_variable& lv = get<local_variable>(bound_temporaries.emplace_back(
        annotated_identifier(name),
        local_variable{ .type = std::move(type), .varid = u.new_variable_identifier(), .is_weak = false }));
    temporaries.emplace_back(name, &lv, el);
    return lv;
}

void prepared_call::export_temporaries(syntax_expression_result& ser)
{
    for(auto& [name, plv, el] : temporaries) {
        ser.temporaries.emplace_back(name, std::move(*plv), el);
    }
}

std::expected<syntax_expression_t, error_storage> deref(fn_compiler_context& ctx, annotated_qname const& aqn)
{
    auto optent = ctx.lookup_entity(aqn);
    using result_t = std::expected<syntax_expression_t, error_storage>;
    return apply_visitor(make_functional_visitor<result_t>([&ctx, &aqn](auto& eid_or_var) -> result_t
    {
        if constexpr (std::is_same_v<std::decay_t<decltype(eid_or_var)>, entity_identifier>) {
            if (!eid_or_var) return std::unexpected(make_error<undeclared_identifier_error>(std::move(aqn)));
            return annotated_entity_identifier{ eid_or_var, aqn.location };
        } else {
            return variable_reference{ aqn, false };
            /*
            semantic::managed_expression_list el{ ctx.u() };
            ctx.u().push_back_expression(el, semantic::push_local_variable{ eid_or_var });
            return indirect_value{
                .location = aqn.location,
                .type = eid_or_var.type,
                .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_list>, el } // bug: el will be removed!
            };
            */
        }
    }), optent);
}

template <size_t SzV>
error_storage append_subarg(fn_compiler_context&, annotated_identifier const* groupname, annotated_entity_identifier argenteid, small_vector<named_expression_t, SzV>& subargs);

template <size_t SzV>
error_storage append_subarg(fn_compiler_context& ctx, annotated_identifier const* groupname, entity_signature const& signature, lex::resource_location const& loc, small_vector<named_expression_t, SzV>& subargs)
{
    unit& u = ctx.u();
    //auto wrong_arg_error = [&arg_loc, eid = *obj]() -> error_storage {
    //    return make_error<basic_general_error>(arg_loc, "wrong ellipsis dereferencing argument"sv, eid);
    //};

    auto nfinds = signature.named_fields_indices();
    auto fields = signature.fields();
    for (uint32_t findex = 0; findex < fields.size(); ++findex) {
        auto const& field = fields[findex];
        BOOST_ASSERT(field.is_const());
        annotated_identifier subgropname;
        if (!groupname) {
            auto nameindex_it = std::ranges::find(nfinds, findex, &std::pair<identifier, uint32_t>::second);
            if (nameindex_it != nfinds.end()) {
                subgropname.value = nameindex_it->first;
                subgropname.location = loc;
                groupname = &subgropname;
            }
        }
        if (auto err = append_subarg(ctx, groupname, annotated_entity_identifier{ field.entity_id(), loc }, subargs); err) {
            return std::move(err);
        }
    }
    return {};
}

template <size_t SzV>
error_storage append_subarg(fn_compiler_context& ctx, annotated_identifier const* groupname, entity const& ent, lex::resource_location const& loc, small_vector<named_expression_t, SzV>& subargs)
{
    unit& u = ctx.u();
    if (empty_entity const* pee = dynamic_cast<empty_entity const*>(&ent); pee) {
        basic_signatured_entity const& ellipsis_arg_type_ent = dynamic_cast<basic_signatured_entity const&>(get_entity(u, pee->get_type()));
        entity_signature const* signature = ellipsis_arg_type_ent.signature();
        if (signature && signature->name == u.get(builtin_qnid::tuple)) {
            return append_subarg(ctx, groupname, *signature, loc, subargs);
        }
    }
    // Not a tuple, add it as is
    if (groupname) {
        subargs.emplace_back(*groupname, annotated_entity_identifier{ ent.id, loc });
    } else {
        subargs.emplace_back(annotated_entity_identifier{ ent.id, loc });
    }
    return {};
}

template <size_t SzV>
error_storage append_subarg(fn_compiler_context& ctx, annotated_identifier const* groupname, annotated_entity_identifier argenteid, small_vector<named_expression_t, SzV>& subargs)
{
    unit& u = ctx.u();
    entity const& arg_ent = get_entity(u, argenteid.value);
    if (qname_entity const* pqne = dynamic_cast<qname_entity const*>(&arg_ent); pqne) {
        auto res = deref(ctx, annotated_qname{ pqne->value(), argenteid.location });
        if (!res) return std::move(res.error());
        if (groupname) {
            subargs.emplace_back(*groupname, std::move(*res));
        } else {
            subargs.emplace_back(std::move(*res));
        }
        return {};
    }
    return append_subarg(ctx, groupname, arg_ent, argenteid.location, subargs);
    /*
    if (empty_entity const* pee = dynamic_cast<empty_entity const*>(&arg_ent); pee) {
        basic_signatured_entity const& arg_type_ent = dynamic_cast<basic_signatured_entity const&>(get_entity(u, pee->get_type()));

    if (basic_signatured_entity const* bse = dynamic_cast<basic_signatured_entity const*>(&arg_ent); bse) {
        if (auto err = append_subarg(ctx, groupname, *bse, argenteid.location, subargs); err) {
            return std::move(err);
        }
    } else {
        if (groupname) {
            subargs.emplace_back(*groupname, argenteid);
        } else {
            subargs.emplace_back(argenteid);
        }
    }

    return {};
    */
}

//void prepared_call::splice_back(semantic::expression_list_t & exprs) const noexcept
//{
//    expressions.splice_back(exprs);
//}
//
//void prepared_call::splice_back(semantic::expression_list_t& exprs, semantic::expression_span span) const noexcept
//{
//    expressions.splice_back(exprs, span);
//}


template <size_t SzV>
struct prepare_call_helper
{
    prepared_call& call;
    small_vector<named_expression_t, SzV> rebuilt_args;
    syntax_expression_t const* pellipsis_arg;
    lex::resource_location arg_loc, arg_expr_loc;

    inline explicit prepare_call_helper(prepared_call& c) noexcept
        : call{ c }
    {}

    void append_arg(annotated_identifier const* groupname, syntax_expression_t&& e)
    {
        //if (rebuilt_args.empty()) {
        //    current_arg->value() = std::move(e);
        //} else 
        if (groupname) {
            rebuilt_args.emplace_back(*groupname, std::move(e));
        } else {
            rebuilt_args.emplace_back(std::move(e));
        }
    }

    error_storage handle_ellipsis_arg(named_expression_t & arg, syntax_expression_t const& ellipsis_arg)
    {
        pellipsis_arg = &ellipsis_arg;
        auto obj = apply_visitor(base_expression_visitor{ call.caller_ctx, call.expressions }, ellipsis_arg);
        if (!obj) return std::move(obj.error());
        auto name_value_tpl = *arg;
        annotated_identifier const* groupname = get<0>(name_value_tpl);
        arg_expr_loc = get_start_location(ellipsis_arg);
        arg_loc = groupname ? groupname->location : arg_expr_loc;

        auto& ser = obj->first;
        return add_subarg(groupname, ser);
    }

    error_storage append_tuple_elements(annotated_identifier const* groupname, syntax_expression_result_t& ser, entity_signature const& tplsig)
    {
        fn_compiler_context& ctx = call.caller_ctx;
        local_variable* ptuple_object_var = nullptr;
        identifier tuple_name;
        if (!ser.is_const_result) {
            BOOST_ASSERT(ser.expressions);
            tuple_name = ctx.u().new_identifier();
            ptuple_object_var = &call.new_temporary(ctx.u(), tuple_name, ser.type(), ser.expressions);
        }
        size_t argpos = 0;
        for (field_descriptor const& fd : tplsig.fields()) {
            annotated_identifier subgropname{ fd.name() };
            annotated_identifier const* fd_groupname = groupname ? groupname : (subgropname ? &subgropname : nullptr);
            if (fd.is_const()) {
                append_arg(fd_groupname, annotated_entity_identifier{ fd.entity_id(), arg_expr_loc });
                if (!fd.name()) ++argpos;
            } else {
                BOOST_ASSERT(ptuple_object_var);
                pure_call_t get_call{ arg_expr_loc };
                get_call.emplace_back(annotated_identifier{ ctx.u().get(builtin_id::self), arg_expr_loc },
                    variable_reference{ annotated_qname{ qname{ tuple_name, false } }, false });
                syntax_expression_t property_value = fd.name() ? syntax_expression_t{ subgropname } : annotated_integer{ mp::integer{ argpos++ } };
                get_call.emplace_back(annotated_identifier{ ctx.u().get(builtin_id::property) }, property_value);
                
                auto match = ctx.find(builtin_qnid::get, get_call, call.expressions);
                if (!match) {
                    return append_cause(
                        make_error<basic_general_error>(arg_expr_loc, "internal error: can't get tuple element"sv, property_value),
                        std::move(match.error()));
                }
                auto res = match->apply(ctx);
                if (!res) {
                    return append_cause(
                        make_error<basic_general_error>(arg_expr_loc, "internal error: can't get tuple element"sv, property_value),
                        std::move(res.error()));
                }
                add_subarg(fd_groupname, *res);
            }
        }
#if 0
        for (auto [name, findex] : tplsig.named_fields_indices()) {
            annotated_identifier subgropname{ name };
            auto const& field = tplsig.fields()[findex];
            if (field.is_const()) {
                append_arg(groupname ? groupname : &subgropname, annotated_entity_identifier{ field.entity_id(), arg_expr_loc });
            } else {
                BOOST_ASSERT(ptuple_object_var);
                pure_call_t get_call{ arg_expr_loc };
                get_call.emplace_back(annotated_identifier{ ctx.u().get(builtin_id::self), arg_expr_loc },
                    variable_reference{ annotated_qname{ qname{ ptuple_object_var->name.value } }, false });
                get_call.emplace_back(annotated_identifier{ ctx.u().get(builtin_id::property) }, subgropname);
                auto match = ctx.find(builtin_qnid::get, get_call);
                if (!match) {
                    return append_cause(
                        make_error<basic_general_error>(arg_expr_loc, "internal error: can't get tuple element"sv, subgropname.value),
                        std::move(match.error()));
                }
                auto res = match->apply(ctx);
                if (!res) {
                    return append_cause(
                        make_error<basic_general_error>(arg_expr_loc, "internal error: can't get tuple element"sv, subgropname.value),
                        std::move(res.error()));
                }
                add_subarg(groupname ? groupname : &subgropname, *res);
            }
        }
        for (size_t argpos = 0; argpos < tplsig.positioned_fields_indices().size(); ++argpos) {
            auto const& field = tplsig.fields()[tplsig.positioned_fields_indices()[argpos]];
            if (field.is_const()) {
                append_arg(groupname, annotated_entity_identifier{ field.entity_id(), arg_expr_loc });
            } else {
                BOOST_ASSERT(ptuple_object_var);
                pure_call_t get_call{ arg_expr_loc };
                get_call.emplace_back(annotated_identifier{ ctx.u().get(builtin_id::self), arg_expr_loc },
                    variable_reference{ annotated_qname{ qname{ ptuple_object_var->name.value } }, false });
                get_call.emplace_back(annotated_identifier{ ctx.u().get(builtin_id::property) }, annotated_integer{ mp::integer{ argpos } });
                auto match = ctx.find(builtin_qnid::get, get_call);
                if (!match) {
                    return append_cause(
                        make_error<basic_general_error>(arg_expr_loc, "internal error: can't get tuple element"sv, annotated_integer{ mp::integer{ argpos } }),
                        std::move(match.error()));
                }
                auto res = match->apply(ctx);
                if (!res) {
                    return append_cause(
                        make_error<basic_general_error>(arg_expr_loc, "internal error: can't get tuple element"sv, annotated_integer{ mp::integer{ argpos } }),
                        std::move(res.error()));
                }
                add_subarg(groupname, *res);
            }
        }
#endif
        return {};
    }

    error_storage add_subarg(annotated_identifier const* groupname, syntax_expression_result_t& ser)
    {
        fn_compiler_context& ctx = call.caller_ctx;
        unit& u = ctx.u();
        entity const* type_ent;
        if (ser.is_const_result) {
            entity const& arg_ent = get_entity(u, ser.value());
            if (qname_entity const* pqne = dynamic_cast<qname_entity const*>(&arg_ent); pqne) {
                auto res = deref(ctx, annotated_qname{ pqne->value(), arg_loc });
                if (!res) return std::move(res.error());
                append_arg(groupname, std::move(*res));
                BOOST_ASSERT(!ser.expressions);
                return {};
            }
            type_ent = &get_entity(u, arg_ent.get_type());
        } else {
            type_ent = &get_entity(u, ser.type());
        }

        entity_signature const* signature = type_ent->signature();
        if (signature && signature->name == u.get(builtin_qnid::tuple)) {
            return append_tuple_elements(groupname, ser, *signature);
        } // else arrays, vectors?

        //return make_error<basic_general_error>(arg_expr_loc, "wrong ellipsis argument type, expected a ct collection"sv, type_ent->id);
        
        if (ser.is_const_result) {
            append_arg(groupname, annotated_entity_identifier{ ser.value(), arg_loc });
        } else {
            semantic::managed_expression_list el{ u };
            el.splice_back(call.expressions, ser.expressions);
            append_arg(groupname, indirect_value{
                .location = arg_loc,
                .type = ser.type(),
                .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_list>, std::move(el) }
            });
        }
        return {};
    }
};

error_storage prepared_call::prepare()
{
    unit& u = caller_ctx.u();
    prepare_call_helper<8> helper{ *this };
    
    for (auto it = args.begin(), eit = args.end(); it != eit; ++it) {
        auto& arg = *it;
        auto name_value_tpl = *arg;
        syntax_expression_t& argvalue = get<1>(name_value_tpl);
        
        auto const* pue = get<unary_expression_t>(&argvalue);
        if (!pue || pue->op != unary_operator_type::ELLIPSIS) {
            if (helper.rebuilt_args.empty()) continue;
            helper.rebuilt_args.emplace_back(std::move(arg));
            continue;
        }
        if (helper.rebuilt_args.empty()) {
            std::move(args.begin(), it, std::back_inserter(helper.rebuilt_args));
        }
        helper.handle_ellipsis_arg(arg, pue->args.front().value());
#if 0
        named_expression_t const& ellipsis_arg = pue->args.front();

        auto obj = apply_visitor(base_expression_visitor { ctx }, ellipsis_arg.value());
        if (!obj) return std::move(obj.error());
            
        auto arg_loc = get<0>(name_value_tpl) ? get<0>(name_value_tpl)->location : get_start_location(argvalue);
        //auto wrong_arg_error = [&arg_loc, eid = *obj]() -> error_storage {
        //    return make_error<basic_general_error>(arg_loc, "wrong ellipsis dereferencing argument"sv, eid);
        //};
        auto append_arg = [&rebuilt_args, &arg, &name_value_tpl](syntax_expression_t&& e) {
            if (rebuilt_args.empty()) {
                arg.value() = std::move(e);
            } else if (get<0>(name_value_tpl)) {
                rebuilt_args.emplace_back(*get<0>(name_value_tpl), std::move(e));
            } else {
                rebuilt_args.emplace_back(std::move(e));
            }
        };

        auto& ser = obj->first;
        if (ser.is_const_result) {
            entity const& ellipsis_arg_ent = get_entity(u, ser.value());
            if (qname_entity const* pqne = dynamic_cast<qname_entity const*>(&ellipsis_arg_ent); pqne) {
                auto res = deref(ctx, annotated_qname{ pqne->value(), arg_loc });
                if (!res) return std::move(res.error());
                append_arg(std::move(*res));
                continue;
            }
            if (empty_entity const* pee = dynamic_cast<empty_entity const*>(&ellipsis_arg_ent); pee) {
                basic_signatured_entity const& ellipsis_arg_type_ent = dynamic_cast<basic_signatured_entity const&>(get_entity(u, pee->get_type()));
                entity_signature const* signature = ellipsis_arg_type_ent.signature();
                if (signature && signature->name == u.get(builtin_qnid::tuple)) {
                    if (rebuilt_args.empty()) {
                        std::move(args.begin(), it, std::back_inserter(rebuilt_args));
                    }
                    if (auto err = append_subarg(ctx, get<0>(name_value_tpl), *signature, arg_loc, rebuilt_args); err) {
                        return std::move(err);
                    }
                    continue;
                }
            }
            append_arg(annotated_entity_identifier{ ser.value(), arg_loc });
        } else {
            entity const& ellipsis_arg_type_ent = get_entity(u, ser.type());
            entity_signature const* signature = ellipsis_arg_type_ent.signature();
            if (!signature || signature->name != u.get(builtin_qnid::tuple)) {
                return make_error<basic_general_error>(arg_loc, "wrong ellipsis dereferencing argument"sv, ellipsis_arg_type_ent.id);
            }
        }
#endif
    }

    if (!helper.rebuilt_args.empty()) {
        args = std::move(helper.rebuilt_args);
    }

    //        entity_signature const& signature = *bse->signature();
    //        if (signature.name != u.get(builtin_qnid::tuple)) return wrong_arg_error();

    //        auto append_subexpr = [optname = get<0>(name_value_tpl), &subargs](syntax_expression_t expr) {
    //            if (optname) {
    //                subargs.emplace_back(optname->value, std::move(expr));
    //            } else {
    //                // to do: take into account subarg's name
    //                subargs.emplace_back(std::move(expr));
    //            }
    //        };

    //        for (auto const& field : bse->signature()->fields()) {
    //            if (!field.is_const()) return wrong_arg_error();
    //            entity const& subarg_ent = get_entity(u, field.entity_id());
    //            if (identifier_entity const* pie = dynamic_cast<identifier_entity const*>(&subarg_ent); pie) {
    //                auto res = deref(ctx, *pie, arg_loc);
    //                if (!res) return std::move(res.error());
    //                append_subexpr(std::move(*res));
    //            } else if (basic_signatured_entity const* sub_bse = dynamic_cast<basic_signatured_entity const*>(&subarg_ent); sub_bse) {
    //                entity_signature const& signature = *bse->signature();
    //                if (signature.name != u.get(builtin_qnid::tuple)) {
    //                    append_subexpr(annotated_entity_identifier{ field.entity_id(), arg_loc });
    //                    continue;
    //                }

    //                THROW_NOT_IMPLEMENTED_ERROR("prepared_call::prepare ellipsis pattern");
    //            }
    //            append_subexpr(annotated_entity_identifier{ field.entity_id(), arg_loc });
    //        }

    //        // Replace current argument with flattened arguments
    //        auto sub_it = subargs.begin(), sub_eit = subargs.end();
    //        if (sub_it != sub_eit) {
    //            arg = std::move(*sub_it);
    //            ++it; ++sub_it;
    //        }
    //        args.insert(it, std::move_iterator(sub_it), std::move_iterator(sub_eit));
    //        std::advance(it, sub_eit - sub_it);
    //        return {};
    //    }
    //    return wrong_arg_error();
    //}

    return {};
}

prepared_call::session prepared_call::new_session(fn_compiler_context& ctxval) const
{
    return session{ ctxval, *this };
}

prepared_call::session::session(fn_compiler_context& ctxval, prepared_call const& c)
    : ctx{ ctxval }
    , call{ c }
{
    // initialize unused named argument
    unused_named_arguments_.reserve(call.named_argument_caches_.size());
    for (auto& [name, cache] : call.named_argument_caches_) {
        unused_named_arguments_.emplace_back(name, &cache);
    }

    // initialize unused position arguments
    unused_position_arguments_.reserve(call.position_argument_caches_.size());
    for (size_t i = 0; i < call.position_argument_caches_.size(); ++i) {
        unused_position_arguments_.emplace_back(&call.position_argument_caches_[i]);
    }
}

std::expected<syntax_expression_result_t, error_storage>
prepared_call::session::do_resolve(argument_cache& arg_cache, expected_result_t const& exp)
{
    auto cit = arg_cache.cache.find({ exp.type, exp.is_const_result });
    if (cit == arg_cache.cache.end()) {
        auto res = apply_visitor(base_expression_visitor{ ctx, call.expressions, exp }, arg_cache.expression);
        if (!res) {
            arg_cache.cache.emplace_hint(cit, cache_key_t{ exp.type, exp.is_const_result }, std::unexpected(res.error()));
            return std::unexpected(std::move(res.error()));
        }
        cit = arg_cache.cache.emplace_hint(cit, cache_key_t{ exp.type, exp.is_const_result }, std::move(res->first));
    }
    return cit->second;
}

std::expected<syntax_expression_result_t, error_storage>
prepared_call::session::use_next_positioned_argument(syntax_expression_t const** pe)
{
    return use_next_positioned_argument(expected_result_t{}, pe);
}

std::expected<syntax_expression_result_t, error_storage>
prepared_call::session::use_next_positioned_argument(expected_result_t const& exp, syntax_expression_t const** pe)
{
    for (;;) {
        if (unused_positioned_index_ >= unused_position_arguments_.size()) {
            return std::unexpected(error_storage{});
        }
    
        // find the argument cache
        argument_cache* arg_cache = unused_position_arguments_[unused_positioned_index_++];
        auto res = do_resolve(*arg_cache, exp);

        if (!res || !res->is_const_result || res->value() != ctx.u().get(builtin_eid::void_)) {
            if (pe) {
                *pe = &arg_cache->expression;
            }
            return res;
        }

        // if the result is void, continue to the next argument
        if (res->expressions) {
            void_spans.emplace_back(res->expressions);
        }
        ++unused_positioned_skipped_;
    }
}

std::expected<syntax_expression_result_t, error_storage>
prepared_call::session::use_named_argument(identifier name, expected_result_t const& exp, syntax_expression_t const** pe)
{
    for (;;) {
        auto argit = std::lower_bound(unused_named_arguments_.begin(), unused_named_arguments_.end(), name, tuple_1st_element_comparator{});
        if (argit == unused_named_arguments_.end() || get<0>(*argit) != name) {
            return std::unexpected(error_storage{});
        }
        argument_cache* arg_cache = get<1>(*argit);
        auto res = do_resolve(*arg_cache, exp);
        if (res) {
            unused_named_arguments_.erase(argit); // remove the argument from the unused list
        }
        if (!res || !res->is_const_result || res->value() != ctx.u().get(builtin_eid::void_)) {
            if (pe) {
                *pe = &arg_cache->expression;
            }
            return res;
        }
        // if the result is void, continue to the next argument
        if (res->expressions) {
            void_spans.emplace_back(res->expressions);
        }
    }
}

named_expression_t prepared_call::session::unused_argument()
{
    while (!unused_named_arguments_.empty()) {
        auto& [name, arg_cache] = unused_named_arguments_.back();
        unused_named_arguments_.pop_back();
        auto res = do_resolve(*arg_cache, expected_result_t{ ctx.u().get(builtin_eid::void_), true });
        if (!res) { // not a void argument
            return named_expression_t{ name, arg_cache->expression };
        }
        if (res->expressions) {
            void_spans.emplace_back(res->expressions);
        }
    }

    while (unused_positioned_index_ < unused_position_arguments_.size()) {
        argument_cache* arg_cache = unused_position_arguments_[unused_positioned_index_++];
        auto res = do_resolve(*arg_cache, expected_result_t{ ctx.u().get(builtin_eid::void_), true });
        if (!res) { // not a void argument
            return named_expression_t{ arg_cache->expression };
        }
        if (res->expressions) {
            void_spans.emplace_back(res->expressions);
        }
        ++unused_positioned_skipped_;
    }

    return {};
}

}
