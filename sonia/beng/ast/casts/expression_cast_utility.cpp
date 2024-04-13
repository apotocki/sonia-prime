//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "utility.hpp"
#include "../expression_implicit_cast_visitor.hpp"

namespace sonia::lang::beng {

//expression_cast_to_vector_visitor::result_type expression_cast_to_vector_visitor::operator()(beng_tuple_t const& v) const
optional<error_storage> cast_elements(fn_compiler_context& ctx, span<const beng_type> from, beng_type to, function<error_context(size_t)> const& cl)
{
    size_t offset = from.size() - 1;
    for (auto t : from) {
        if (t != to) {
            if (offset) ctx.append_expression(semantic::push_by_offset{ offset });
            expression_implicit_cast_visitor vis{ ctx, t, [index = from.size() - offset - 1, &cl] { return cl(index); } };
            auto r = apply_visitor(vis, to);
            if (!r.has_value()) {
                return r.error();
            }
            if (offset) {
                ctx.append_expression(semantic::set_by_offset{ offset + 1 });
                ctx.append_expression(semantic::truncate_values{ 1, false });
            }
        }
        --offset;
    }
    return nullopt;
}

}
