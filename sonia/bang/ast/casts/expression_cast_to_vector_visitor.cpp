//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "expression_cast_to_vector_visitor.hpp"
//#include "../expression_implicit_cast_visitor.hpp"
#include "utility.hpp"

namespace sonia::lang::bang {

expression_cast_to_vector_visitor::result_type expression_cast_to_vector_visitor::operator()(bang_tuple_t const& v) const
{
    if (v.named_fields.empty()) {
        bool compatible = true;
        for (auto t : v.fields) {
            if (t != target.type) {
                compatible = false;
                break;
            }
        }
        // to do: compatible tuple to vector cast
        if (compatible) {
            ctx.context_type = target;
            return {};
        }
        // try to cast
        if (!v.unpacked) {
            ctx.append_expression(ctx.u().get_builtin_function(unit::builtin_fn::unpack));
        }
        auto opterror = cast_elements(ctx, v.fields, target.type, [this](size_t index) -> error_context {
            error_context ctx = cl_();
            if (auto opte = ctx.expression(); opte) {
                return { get<expression_vector_t>(*opte) };
            } else {
                return { ctx.location() };
            }
        });
        ctx.append_expression(semantic::push_value{ decimal{ v.fields.size() } });
        ctx.append_expression(ctx.u().get_builtin_function(unit::builtin_fn::arrayify));
        if (!opterror) {
            ctx.context_type = target;
            return {};
        }
    }
    return make_error<cast_error>(cl_(), target, v);
}

}
