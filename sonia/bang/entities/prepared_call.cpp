//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include "prepared_call.hpp"

#include "sonia/sal.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/errors.hpp"

#include "sonia/bang/errors/no_position_argument_error.hpp"

#define BANG_SKIP_VOID_ARGUMENTS

namespace sonia::lang::bang {

prepared_call::prepared_call(fn_compiler_context& ctx, functional const* pf, semantic::expression_list_t& ael, lex::resource_location loc) noexcept
    : caller_ctx{ ctx }
    , pfnl{ pf }
    , expressions{ ael }
    , location{ std::move(loc) }
{
    init_bindings();
}

prepared_call::prepared_call(fn_compiler_context &ctx, functional const* pf, pure_call_t const& call, semantic::expression_list_t& ael)
    : caller_ctx{ ctx }
    , pfnl{ pf }
    , expressions{ ael }
    , location{ call.location }
    , args{ call.args }
{
    init_bindings();
}

void prepared_call::init_bindings()
{
    caller_ctx.push_binding(bound_temporaries);
}

prepared_call::~prepared_call()
{
    caller_ctx.pop_binding(&bound_temporaries);
}

qname_view prepared_call::functional_name() const noexcept
{
    return pfnl ? pfnl->name() : qname_view{};
}

qname_identifier prepared_call::functional_id() const noexcept
{
    return pfnl ? pfnl->id() : qname_identifier{};
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
        }
    }), optent);
}

error_storage prepared_call::prepare()
{
    unit& u = caller_ctx.u();
    small_vector<named_expression_t, 8> rebuilt_args;
    bool use_rebuilt_args = false;

    auto append_arg = [&rebuilt_args](annotated_identifier const* groupname, syntax_expression_t&& e) {
        if (groupname) {
            rebuilt_args.emplace_back(*groupname, std::move(e));
        } else {
            rebuilt_args.emplace_back(std::move(e));
        }
    };

    for (auto it = args.begin(), eit = args.end(); it != eit; ++it) {
        auto& arg = *it;
        auto name_value_tpl = *arg;
        syntax_expression_t& argvalue = get<1>(name_value_tpl);
        
        auto * pue = get<unary_expression_t>(&argvalue);
        if (!pue || pue->op != unary_operator_type::ELLIPSIS) {
            if (!use_rebuilt_args) continue;
            rebuilt_args.emplace_back(std::move(arg));
            continue;
        }
        if (!use_rebuilt_args) {
            std::move(args.begin(), it, std::back_inserter(rebuilt_args));
            use_rebuilt_args = true;
        }
        syntax_expression_t & ellipsis_arg = pue->args.front().value();
        auto obj = apply_visitor(base_expression_visitor{ caller_ctx, expressions }, ellipsis_arg);
        if (!obj) return std::move(obj.error());
        
        annotated_identifier const* groupname = get<0>(name_value_tpl);
        lex::resource_location arg_expr_loc = get_start_location(ellipsis_arg);
        lex::resource_location arg_loc = groupname ? groupname->location : arg_expr_loc;
        
        auto& ser = obj->first;
        entity_identifier type_eid;
        if (ser.is_const_result) {
            entity const& arg_ent = get_entity(u, ser.value());
            if (qname_entity const* pqne = dynamic_cast<qname_entity const*>(&arg_ent); pqne) {
                auto res = deref(caller_ctx, annotated_qname{ pqne->value(), arg_loc });
                if (!res) return std::move(res.error());
                append_arg(groupname, std::move(*res));
                continue;
            }
            type_eid = arg_ent.get_type();
        } else {
            type_eid = ser.type();
        }

        entity const& type_ent = get_entity(u, type_eid);
        entity_signature const* signature = type_ent.signature();
        if (!signature || signature->name != u.get(builtin_qnid::tuple)) {
            if (ser.is_const_result) {
                append_arg(groupname, annotated_entity_identifier{ ser.value(), arg_loc });
            } else {
                append_arg(groupname, std::move(ellipsis_arg));
            }
            continue;
        }

        // tuple signature, append its elements
        local_variable* ptuple_object_var = nullptr;
        identifier tuple_name;
        if (!ser.is_const_result) {
            BOOST_ASSERT(ser.expressions);
            tuple_name = u.new_identifier();
            ptuple_object_var = &new_temporary(u, tuple_name, ser.type(), ser.expressions);
        }
        size_t argpos = 0;
        for (field_descriptor const& fd : signature->fields()) {
            annotated_identifier subgropname{ fd.name() };
            annotated_identifier const* fd_groupname = groupname ? groupname : (subgropname ? &subgropname : nullptr);
            if (fd.is_const()) {
                entity const& arg_ent = get_entity(u, fd.entity_id());
                if (qname_entity const* pqne = dynamic_cast<qname_entity const*>(&arg_ent); pqne) {
                    auto res = deref(caller_ctx, annotated_qname{ pqne->value(), arg_expr_loc });
                    if (!res) return std::move(res.error());
                    append_arg(fd_groupname, std::move(*res));
                } else {
                    append_arg(fd_groupname, annotated_entity_identifier{ fd.entity_id(), arg_expr_loc });
                }
            } else {
                BOOST_ASSERT(ptuple_object_var);
                pure_call_t get_call{ arg_expr_loc };
                get_call.emplace_back(annotated_identifier{ u.get(builtin_id::self), arg_expr_loc },
                    variable_reference{ annotated_qname{ qname{ tuple_name, false } }, false });
                get_call.emplace_back(annotated_identifier{ u.get(builtin_id::property) }, annotated_integer{ mp::integer{ argpos } });
                
                auto match = caller_ctx.find(builtin_qnid::get, get_call, expressions);
                if (!match) {
                    return append_cause(
                        make_error<basic_general_error>(arg_expr_loc, "internal error: can't get tuple element"sv, annotated_integer{ mp::integer{ argpos - 1 } }),
                        std::move(match.error()));
                }
                auto res = match->apply(caller_ctx);
                if (!res) {
                    return append_cause(
                        make_error<basic_general_error>(arg_expr_loc, "internal error: can't get tuple element"sv, annotated_integer{ mp::integer{ argpos - 1 } }),
                        std::move(res.error()));
                }

                //add_subarg(fd_groupname, *res);
                syntax_expression_result_t& elem_ser = *res;
                BOOST_ASSERT(!elem_ser.is_const_result);
                //if (elem_ser.is_const_result) {
                //    append_arg(fd_groupname, annotated_entity_identifier{ elem_ser.value(), arg_expr_loc });
                //} else {

                // if element is named, extract it's value type
                entity_identifier elem_type_eid = elem_ser.type();
                if (subgropname) {
                    entity const& elem_type_ent = get_entity(u, elem_ser.type());
                    entity_signature const* elem_signature = elem_type_ent.signature();
                    BOOST_ASSERT(elem_signature && elem_signature->name == u.get(builtin_qnid::tuple) && elem_signature->field_count() == 2);
                    BOOST_ASSERT(!elem_signature->fields()[1].is_const());
                    elem_type_eid = elem_signature->fields()[1].entity_id();
                } 
                semantic::managed_expression_list el{ u };
                el.splice_back(expressions, elem_ser.expressions);
                append_arg(fd_groupname, indirect_value{
                    .location = arg_expr_loc,
                    .type = elem_type_eid,
                    .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_list>, std::move(el) }
                });
            }
            ++argpos;
        }
    }

    if (use_rebuilt_args) {
        args = std::move(rebuilt_args);
    }

    // initialize caches
    argument_caches_.reserve(args.size());
    size_t arg_cnt = argument_caches_.size();
    if (arg_cnt > std::numeric_limits<uint64_t>::digits - 1) {
        THROW_NOT_IMPLEMENTED_ERROR("Too many arguments in prepared call");
    }

    named_map_ = positioned_map_ = 0;
    uint64_t argbit = 1;
    for (auto const& arg : args) {
        auto [name, expr] = *arg;
        argument_caches_.emplace_back(name ? name->value : identifier{}, expr);
        if (name) {
            named_map_ |= argbit;
        } else {
            positioned_map_ |= argbit;
        }
        argbit <<= 1;
    }

    //    if (name) {
    //        named_argument_caches_.emplace_back(name->value, argument_cache{ expr });
    //    }
    //    else {
    //        position_argument_caches_.emplace_back(expr);
    //    }
    //}
    //// sort named arguments
    //std::ranges::sort(named_argument_caches_, {}, [](auto const& pair) { return get<0>(pair).value; });
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
    named_usage_map_ = call.named_map_;
    positioned_usage_map_ = call.positioned_map_;
    
    //// initialize unused named argument
    //unused_named_arguments_.reserve(call.named_argument_caches_.size());
    //for (auto& [name, cache] : call.named_argument_caches_) {
    //    unused_named_arguments_.emplace_back(name, &cache);
    //}

    //// initialize unused position arguments
    //unused_position_arguments_.reserve(call.position_argument_caches_.size());
    //for (size_t i = 0; i < call.position_argument_caches_.size(); ++i) {
    //    unused_position_arguments_.emplace_back(&call.position_argument_caches_[i]);
    //}
}

std::expected<std::pair<syntax_expression_result_t, bool>, error_storage>
prepared_call::session::do_resolve(argument_cache& arg_cache, expected_result_t const& exp)
{
    auto cit = arg_cache.cache.find({ exp.type, exp.modifier & parameter_constraint_modifier_t::const_or_runtime_type });
    if (cit == arg_cache.cache.end()) {
        auto res = apply_visitor(base_expression_visitor{ ctx, call.expressions, exp }, arg_cache.expression);
        cit = arg_cache.cache.emplace_hint(cit, cache_key_t{ exp.type, exp.modifier & parameter_constraint_modifier_t::const_or_runtime_type }, res);
        if (exp.can_be_only_constexpr() || exp.can_be_only_runtime_type()) {
            arg_cache.cache.emplace(cache_key_t{ exp.type, parameter_constraint_modifier_t::const_or_runtime_type }, res);
        } else if (exp.can_be_constexpr() && res->first.is_const_result) {
            // if the expression is const result, we can cache it with const_or_runtime_type modifier
            arg_cache.cache.emplace(cache_key_t{ exp.type, parameter_constraint_modifier_t::const_type }, res);
        } else if (exp.can_be_runtime_type() && !res->first.is_const_result) {
            arg_cache.cache.emplace(cache_key_t{ exp.type, parameter_constraint_modifier_t::runtime_type }, res);
        }
    }
    return cit->second;
}

void prepared_call::session::reuse_argument(size_t argindex)
{
    if (get<0>(call.argument_caches_[argindex])) {
        // if the argument is named, add it back to the named usage map
        named_usage_map_ |= (1ull << argindex);
    } else {
        // if the argument is positioned, add it back to the positioned usage map
        positioned_usage_map_ |= (1ull << argindex);
    }
}

bool prepared_call::session::has_more_positioned_arguments() const noexcept
{
    return positioned_usage_map_ != 0;
}

std::expected<std::pair<syntax_expression_result_t, bool>, error_storage>
prepared_call::session::use_next_positioned_argument(std::pair<syntax_expression_t const*, size_t>* pe)
{
    return use_next_positioned_argument(expected_result_t{}, pe);
}

std::expected<std::pair<syntax_expression_result_t, bool>, error_storage>
prepared_call::session::use_next_positioned_argument(expected_result_t const& exp, std::pair<syntax_expression_t const*, size_t> * pe)
{
    while (positioned_usage_map_) {
        // get next unused argument index
        uint64_t pow2_argindex = positioned_usage_map_ - ((positioned_usage_map_ - 1) & positioned_usage_map_);
        uint8_t argindex = sonia::sal::log2(pow2_argindex);
        positioned_usage_map_ -= pow2_argindex; // remove the argument from the usage map

        // find the argument cache
        auto& [name, arg_cache] = call.argument_caches_[argindex];
        BOOST_ASSERT(!name);

        auto res = do_resolve(arg_cache, exp);

#ifdef BANG_SKIP_VOID_ARGUMENTS
        if (pe) {
            *pe = { &arg_cache.expression, argindex };
        }
        return res;
#else
        if (!res || !res->first.is_const_result || res->first.value() != ctx.u().get(builtin_eid::void_)) {
            if (pe) {
                *pe = { &arg_cache.expression, argindex };
            }
            return res;
        }

        // if the result is void, continue to the next argument
        if (res->first.expressions) {
            void_spans.emplace_back(res->first.expressions);
        }
#endif
    }

    // no more positioned unused arguments
    return std::unexpected(error_storage{});
}

std::expected<std::pair<syntax_expression_result_t, bool>, error_storage>
prepared_call::session::use_named_argument(identifier name, expected_result_t const& exp, std::pair<syntax_expression_t const*, size_t>* pe)
{
    for (auto tmp_map = named_usage_map_; tmp_map;) {
        // get next unused argument index
        uint64_t pow2_argindex = tmp_map - ((tmp_map - 1) & tmp_map);
        uint8_t argindex = sonia::sal::log2(pow2_argindex);
        tmp_map -= pow2_argindex; // remove the argument from the usage map

        // find the argument cache
        auto& [argname, arg_cache] = call.argument_caches_[argindex];
        BOOST_ASSERT(argname);

        if (name != argname) continue; // not the argument we are looking for
        
        auto res = do_resolve(arg_cache, exp);
#ifdef BANG_SKIP_VOID_ARGUMENTS
        if (pe) {
            *pe = { &arg_cache.expression, argindex };
        }
        named_usage_map_ -= pow2_argindex;
        return res;
#else
        if (!res || !res->first.is_const_result || res->first.value() != ctx.u().get(builtin_eid::void_)) {
            if (pe) {
                *pe = { &arg_cache.expression, argindex };
            }
            named_usage_map_ -= pow2_argindex;
            return res;
        }

        // if the result is void, continue to the next argument
        if (res->first.expressions) {
            void_spans.emplace_back(res->first.expressions);
        }
#endif
    }
    // no more named unused arguments
    return std::unexpected(error_storage{});
}

std::expected<std::pair<syntax_expression_result_t, bool>, error_storage>
prepared_call::session::use_next_argument(expected_result_t const& exp, std::tuple<identifier, syntax_expression_t const*, size_t>* pe)
{
    for (auto tmp_map = named_usage_map_ | positioned_usage_map_; tmp_map;) {
        uint64_t pow2_argindex = tmp_map - ((tmp_map - 1) & tmp_map);
        uint8_t argindex = sonia::sal::log2(pow2_argindex);

        named_usage_map_ &= ~pow2_argindex; // remove the argument from the named usage map
        positioned_usage_map_ &= ~pow2_argindex; // remove the argument from the positioned usage map

        // find the argument cache
        auto& [argname, arg_cache] = call.argument_caches_[argindex];

        auto res = do_resolve(arg_cache, exp);
#ifdef BANG_SKIP_VOID_ARGUMENTS
        if (pe) {
            *pe = { argname, &arg_cache.expression, argindex };
        }
        return res;
#else
        if (!res || !res->first.is_const_result || res->first.value() != ctx.u().get(builtin_eid::void_)) {
            if (pe) {
                *pe = { argname, &arg_cache.expression, argindex };
            }
            return res;
        }

        // if the result is void, continue to the next argument
        if (res->first.expressions) {
            void_spans.emplace_back(res->first.expressions);
        }

        tmp_map -= pow2_argindex;
#endif
    }

    // no more named unused arguments
    return std::unexpected(error_storage{});
}

named_expression_t prepared_call::session::unused_argument()
{
    for (auto tmp_map = named_usage_map_ | positioned_usage_map_; tmp_map;) {
        uint64_t pow2_argindex = tmp_map - ((tmp_map - 1) & tmp_map);
        uint8_t argindex = sonia::sal::log2(pow2_argindex);

        auto& [argname, arg_cache] = call.argument_caches_[argindex];
#ifdef BANG_SKIP_VOID_ARGUMENTS
        return argname ? named_expression_t{ argname, arg_cache.expression } : named_expression_t{ arg_cache.expression };
#else
        auto res = do_resolve(arg_cache, expected_result_t{ .type = ctx.u().get(builtin_eid::void_), .modifier = parameter_constraint_modifier_t::const_type });
        if (!res) { // not a void argument
            return argname ? named_expression_t{ argname, arg_cache.expression } : named_expression_t{ arg_cache.expression };
        }
        if (res->first.expressions) {
            void_spans.emplace_back(res->first.expressions);
        }
        tmp_map -= pow2_argindex; // remove the argument from the usage map
        named_usage_map_ &= ~pow2_argindex; // remove the argument from the named usage map
        positioned_usage_map_ &= ~pow2_argindex; // remove the argument from the positioned usage map
#endif
    }

    return {};
}

}
