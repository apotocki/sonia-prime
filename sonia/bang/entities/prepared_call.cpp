//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include "prepared_call.hpp"

#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/errors.hpp"

#include "sonia/bang/errors/no_position_argument_error.hpp"

namespace sonia::lang::bang {

prepared_call::prepared_call(pure_call_t const& call)
    : location{ call.location }
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
            semantic::managed_expression_list el{ ctx.u() };
            ctx.u().push_back_expression(el, semantic::push_local_variable{ eid_or_var });
            return indirect_value{
                .location = aqn.location,
                .type = eid_or_var.type,
                .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_list>, std::move(el) }
            };
        }
    }), optent);
}

template <size_t SzV>
error_storage append_subarg(fn_compiler_context&, annotated_identifier const* groupname, annotated_entity_identifier argenteid, small_vector<named_expression_t, SzV>& subargs);

template <size_t SzV>
error_storage append_subarg(fn_compiler_context& ctx, annotated_identifier const* groupname, basic_signatured_entity const& tpl, lex::resource_location const& loc, small_vector<named_expression_t, SzV>& subargs)
{
    unit& u = ctx.u();
    //auto wrong_arg_error = [&arg_loc, eid = *obj]() -> error_storage {
    //    return make_error<basic_general_error>(arg_loc, "wrong ellipsis dereferencing argument"sv, eid);
    //};
    entity_signature const& signature = *tpl.signature();

    if (signature.name != u.get(builtin_qnid::tuple) || tpl.get_type() == u.get(builtin_eid::typename_)) {
        // Not a tuple, add it as is
        if (groupname) {
            subargs.emplace_back(*groupname, annotated_entity_identifier{ tpl.id(), loc });
        } else {
            subargs.emplace_back(annotated_entity_identifier{ tpl.id(), loc });
        }
    }
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
    }
    else if (basic_signatured_entity const* bse = dynamic_cast<basic_signatured_entity const*>(&arg_ent); bse) {
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
}

void prepared_call::splice_back(semantic::expression_list_t & exprs) const noexcept
{
    expressions.splice_back(exprs);
}

void prepared_call::splice_back(semantic::expression_list_t& exprs, semantic::expression_span span) const noexcept
{
    expressions.splice_back(exprs, span);
}

error_storage prepared_call::prepare(fn_compiler_context& ctx)
{
    unit& u = ctx.u();
    small_vector<named_expression_t, 8> rebuilt_args;
    for (auto it = args.begin(), eit = args.end(); it != eit; ++it) {
        auto& arg = *it;
        auto name_value_tpl = *arg;
        syntax_expression_t& argvalue = get<1>(name_value_tpl);
        
        auto const* pue = get<unary_expression_t>(&argvalue);
        if (!pue || pue->op != unary_operator_type::ELLIPSIS) {
            if (rebuilt_args.empty()) continue;
            rebuilt_args.emplace_back(std::move(arg));
            continue;
        }

        named_expression_t const& ellipsis_arg = pue->args.front();
        auto obj = apply_visitor(ct_expression_visitor { ctx }, ellipsis_arg.value());
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

        entity const& ellipsis_arg_ent = get_entity(u, obj->value);
        if (qname_entity const* pqne = dynamic_cast<qname_entity const*>(&ellipsis_arg_ent); pqne) {
            auto res = deref(ctx, annotated_qname{ pqne->value(), arg_loc });
            if (!res) return std::move(res.error());
            append_arg(std::move(*res));
        } else if (basic_signatured_entity const* bse = dynamic_cast<basic_signatured_entity const*>(&ellipsis_arg_ent); bse) {
            if (rebuilt_args.empty()) {
                std::move(args.begin(), it, std::back_inserter(rebuilt_args));
            }
            if (auto err = append_subarg(ctx, get<0>(name_value_tpl), *bse, arg_loc, rebuilt_args); err) {
                return std::move(err);
            }
        } else {
            append_arg(annotated_entity_identifier(obj->value, arg_loc));
        }
    }
    if (!rebuilt_args.empty()) {
        args = std::move(rebuilt_args);
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
    : ctx{ ctxval }, call{ c }
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

std::expected<syntax_expression_result_reference_t, error_storage>
prepared_call::session::do_resolve(argument_cache& arg_cache, annotated_entity_identifier const& exp, bool const_exp)
{
    auto cit = arg_cache.cache.find({ exp.value, const_exp });
    if (cit == arg_cache.cache.end()) {
        auto res = apply_visitor(base_expression_visitor{ ctx, exp, const_exp }, arg_cache.expression);
        if (!res) {
            arg_cache.cache.emplace_hint(cit, cache_key_t{ exp.value, const_exp }, std::unexpected(res.error()));
            return std::unexpected(std::move(res.error()));
        }
        syntax_expression_result_t & er = res->first;
        semantic::expression_span sp = er.expressions;
        call.splice_back(er.expressions);
        cit = arg_cache.cache.emplace_hint(cit, cache_key_t{ exp.value, const_exp }, syntax_expression_result_reference_t{
            .expressions = std::move(sp),
            .value_or_type = er.value_or_type,
            .is_const_result = er.is_const_result
        });
    }
    return cit->second;
}

std::expected<syntax_expression_result_reference_t, error_storage>
prepared_call::session::use_next_positioned_argument(annotated_entity_identifier const& exp, bool const_exp)
{
    for (;;) {
        if (unused_positioned_index_ >= unused_position_arguments_.size()) {
            return std::unexpected(make_error<no_position_argument_error>(call.location, unused_positioned_index_ - unused_positioned_skipped_));
        }
    
        // find the argument cache
        argument_cache* arg_cache = unused_position_arguments_[unused_positioned_index_++];
        auto res = do_resolve(*arg_cache, exp, const_exp);

        if (!res || !res->is_const_result || res->value() != ctx.u().get(builtin_eid::void_)) {
            return res;
        }

        // if the result is void, continue to the next argument
        if (res->expressions) {
            void_spans.emplace_back(res->expressions);
        }
        ++unused_positioned_skipped_;
    }
}

named_expression_t prepared_call::session::unused_argument()
{
    // to do: deal with named arguments

    while (unused_positioned_index_ < unused_position_arguments_.size()) {
        argument_cache* arg_cache = unused_position_arguments_[unused_positioned_index_++];
        auto res = do_resolve(*arg_cache, annotated_entity_identifier{ ctx.u().get(builtin_eid::void_) }, true);
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

 