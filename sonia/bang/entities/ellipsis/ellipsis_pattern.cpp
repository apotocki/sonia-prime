//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "ellipsis_pattern.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

//#include "pack_entity.hpp"

namespace sonia::lang::bang {

class ellipsis_match_descriptor : public functional_match_descriptor
{
    variant<identifier_entity const*, basic_signatured_entity const*> arg_;

public:
    template <typename EntityT>
    inline ellipsis_match_descriptor(unit& u, EntityT const* ent, lex::resource_location const& loc) noexcept
        : functional_match_descriptor{ u }
        , arg_{ ent }
    {
        location = loc;
    }

    auto const& argument() const noexcept { return arg_; }
};

// postfix operator ...(__identifier|metaobject(__identifier...))
std::expected<functional_match_descriptor_ptr, error_storage> ellipsis_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    unit& u = ctx.u();
    syntax_expression_t const* object_arg = nullptr;

    for (auto const& arg : call.args()) {
        annotated_identifier const* pargname = arg.name();
        if (pargname) {
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, unexpected argument"sv, *pargname));
        }
        syntax_expression_t const& e = arg.value();
        if (object_arg) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(e), "argument mismatch, unexpected argument"sv, e));
        }
        object_arg = &e;
    }

    if (!object_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched parameter"sv));
    }

    ct_expression_visitor eobjvis{ ctx };
    auto obj = apply_visitor(eobjvis, *object_arg);
    if (!obj) return std::unexpected(std::move(obj.error()));

    entity const& metaobject_ent = u.eregistry_get(*obj);
    if (identifier_entity const* pie = dynamic_cast<identifier_entity const*>(&metaobject_ent); pie) {
        return make_shared<ellipsis_match_descriptor>(u, pie, get_start_location(*object_arg));
    }
    else if (basic_signatured_entity const* bse = dynamic_cast<basic_signatured_entity const*>(&metaobject_ent); bse) {
        entity_signature const& signature = *bse->signature();
        if (signature.name == u.get(builtin_qnid::metaobject)) {
            // to do: check if the signature is a comopatible to the call metaobject
            return make_shared<ellipsis_match_descriptor>(u, bse, get_start_location(*object_arg));
        }
    }
    
    return std::unexpected(make_error<basic_general_error>(call.location(), "argument mismatch"sv, *object_arg));
}

std::expected<entity_identifier, error_storage> ellipsis_pattern::const_apply(fn_compiler_context& ctx, qname_identifier fid, functional_match_descriptor& md) const
{
    THROW_NOT_IMPLEMENTED_ERROR("ellipsis_pattern::const_apply");
}

error_storage ellipsis_pattern::apply(fn_compiler_context& ctx, qname_identifier fid, functional_match_descriptor& md) const
{
    BOOST_ASSERT(dynamic_cast<ellipsis_match_descriptor*>(&md));

    ellipsis_match_descriptor& nsmd = static_cast<ellipsis_match_descriptor&>(md);

    return apply_visitor(make_functional_visitor<error_storage>([&ctx, &nsmd](auto const* pe) -> error_storage {
        if constexpr (std::is_same_v<decltype(pe), identifier_entity const*>) {
            annotated_qname varname{ qname{ pe->value(), false }, nsmd.location };
            auto opteid = ctx.lookup_entity(varname);
            if (!opteid) return std::move(opteid.error());
            if (!*opteid) return make_error<undeclared_identifier_error>(std::move(varname));
            
            entity const& ref_ent = ctx.u().eregistry_get(*opteid);
            if (variable_entity const* vent = dynamic_cast<variable_entity const*>(&ref_ent); vent) {
                ctx.append_expression(semantic::push_value{ *opteid });
                ctx.context_type = ref_ent.get_type();
            } else {
                THROW_NOT_IMPLEMENTED_ERROR("ellipsis_pattern::apply(not a variable)");
                //return make_error<basic_general_error>(nsmd.location, "not a variable"sv, varname);
            }
            return error_storage{};
            
        } else {
            THROW_NOT_IMPLEMENTED_ERROR("ellipsis_pattern::apply(metaobject)");
        }
    }), nsmd.argument());
}

}
