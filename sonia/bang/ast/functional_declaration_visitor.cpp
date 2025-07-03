//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "functional_declaration_visitor.hpp"

#include "fn_compiler_context.hpp"

namespace sonia::lang::bang {

inline unit& functional_declaration_visitor::u() const noexcept { return ctx.u(); }

error_storage functional_declaration_visitor::apply(span<const statement> initial_decls) const
{
    decl_stack_.clear();
    decl_stack_.emplace_back(initial_decls);
    for (; !decl_stack_.empty(); ) {
        if (decl_stack_.back().empty()) {
            decl_stack_.pop_back();
            continue;
        }
        size_t index = decl_stack_.size() - 1;
        if (auto err = apply_visitor(*this, decl_stack_.back().front()); err) return err;
        decl_stack_[index] = decl_stack_[index].subspan(1);
    }
    return {};
}

}
