//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "functional.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/ellipsis/pack_entity.hpp"

namespace sonia::lang::bang {

function_descriptor::named_field const* function_descriptor::find_named_field(identifier name) const
{
    auto it = std::lower_bound(nfields_.begin(), nfields_.end(), name,
        [](named_field const& l, identifier r) { return l.ename.value < r; });
    if (it != nfields_.end() && it->ename.value == name) return &*it;
    return nullptr;
}

std::expected<functional::pattern const*, error_storage> functional::find(fn_compiler_context& ctx, pure_call_t const& call, functional::match_descriptor & md) const
{
    alt_error err;
    mp::decimal priority = 0;
    using alternative_t = std::pair<pattern const*, fn_compiler_context::expr_vec_t>;
    boost::container::small_vector<alternative_t, 2> alternatives;
    auto estate = ctx.expressions_state();
    fn_compiler_context::expr_vec_t branch;

    for (auto const& p : patterns_) {
        auto cmp = priority <=> p->get_weight();
        if (cmp == std::strong_ordering::greater) continue;
        ctx.push_chain(branch);
        match_descriptor temp_md{ {}, entity_signature{id_} };
        if (auto res = p->is_matched(ctx, call, temp_md); res) {
            if (alternatives.empty()) { // no need to store errors if a match exists
                //err.alternatives.emplace_back(
                //    make_error<function_call_match_error>(annotated_qname_identifier{ id_, call.location() }, sig, std::move(optres))
                //);
                err.alternatives.emplace_back(std::move(res));
            }
        } else {
            if (cmp == std::strong_ordering::less) {
                priority = p->get_weight();
                md = std::move(temp_md);
                alternatives.clear();
            }
            alternatives.emplace_back(p.get(), std::move(branch));
        }
        estate.restore();
        branch.clear();
    }

    if (alternatives.empty()) {
        if (err.alternatives.size() == 1) {
            return std::unexpected(std::move(err.alternatives.front()));
        }
        if (err.alternatives.empty()) {
            return std::unexpected(make_error<function_call_match_error>(annotated_qname_identifier{ id_, call.location() }, nullptr));
        }
        return std::unexpected(make_error<alt_error>(std::move(err)));
    }
    if (alternatives.size() > 1) {
        return std::unexpected(make_error<ambiguity_error>());
    }
    estate.detach();
    ctx.append_expression(std::move(alternatives.front().second));
    fn_compiler_context::expr_vec_t& args = get<fn_compiler_context::expr_vec_t>(ctx.expressions().back());
    ctx.push_chain(args);
    return &*alternatives.front().first;
}

error_storage fieldset_pattern::is_matched(fn_compiler_context& ctx, pure_call_t const& call, functional::match_descriptor&) const
{
    for (auto const& tpl : call.named_args) { // {argname, expr, exprloc}
        THROW_NOT_IMPLEMENTED_ERROR("fn_pattern::is_matched, named argument");
    }
    fieldset_t const& fs = get_fieldset();
    auto psp = fs.positioned_fields();
    auto posarg = psp.begin();

    THROW_NOT_IMPLEMENTED_ERROR("fn_pattern::is_matched");
#if 0
    for (auto expr_it = call.positioned_args.begin(), expr_eit = call.positioned_args.end(); expr_it != expr_eit;) {
        if (posarg == psp.end()) {
            return make_error<basic_general_error>(get_start_location(*expr_it), "positioned argument mismatch"sv);
        }
        entity_identifier param_type = posarg->constraint;
        entity const& param_entity = ctx.u().eregistry().get(param_type);
        if (pack_entity const* pent = dynamic_cast<pack_entity const*>(&param_entity); pent) {
            uint64_t pack_size = 0;
            //ctx.append_expression(semantic::push_value{ uint64_t{pack_size} });
            //auto pack_size_expr_pointer = ctx.current_expressions_pointer();
            for (;;) {
                expression_visitor evis{ ctx, expected_result_t{ pent->element_type(), get_start_location(*expr_it) } };
                if (auto opterr = apply_visitor(evis, *expr_it); opterr) {
                    ++posarg;
                    if (posarg == psp.end()) return std::move(opterr);
                    break;
                }
                ++pack_size;
                if (++expr_it == expr_eit) {
                    ++posarg;
                    break;
                }
            }
            ctx.append_expression(semantic::push_value{ uint64_t{pack_size} });
            //ctx.set_expression(pack_size_expr_pointer, semantic::push_value{ uint64_t{pack_size} });
        } else {
            expression_visitor evis{ ctx, expected_result_t{ param_type, get_start_location(*expr_it) } };
            if (auto opterr = apply_visitor(evis, *expr_it); opterr) return std::move(opterr);
            ++expr_it;
            ++posarg;
        }
    }
    while (posarg != psp.end()) {
        entity_identifier param_type = posarg->constraint;
        entity const& param_entity = ctx.u().eregistry().get(param_type);
        if (pack_entity const* pent = dynamic_cast<pack_entity const*>(&param_entity); pent) {
            ctx.append_expression(semantic::push_value{ uint64_t{0} }); // set empty argument pack
            ++posarg;
            continue;
        }
        return make_error<basic_general_error>(call.location(), "positioned argument mismatch"sv);
    }
    // to do: replace expressions according signature
    return {};
#endif
}

std::expected<entity_identifier, error_storage> functional::pattern::apply(fn_compiler_context& ctx, match_descriptor& md) const
{
    auto r = const_apply(ctx, md);
    if (!r.has_value()) return std::unexpected(std::move(r.error()));
    ctx.append_expression(semantic::push_value{r.value()});
    entity const& e = ctx.u().eregistry().get(r.value());
    return e.get_type();
}

std::expected<entity_identifier, error_storage> fieldset_pattern::const_apply(fn_compiler_context& ctx, functional::match_descriptor&) const
{
    // , std::span<semantic::expression_type> args
    THROW_NOT_IMPLEMENTED_ERROR("fn_pattern::const_apply");
}




}
