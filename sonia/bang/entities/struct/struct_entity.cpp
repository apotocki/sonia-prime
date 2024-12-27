//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "struct_entity.hpp"

namespace sonia::lang::bang {

    void struct_entity::build(fn_compiler_context& ctx)
    {
        if (built_) return;
        built_ = true;
        for (auto& f : fields_) {
            //auto& fent = ctx.u().resolve_functional(f.entity_id());
            //if (f.is_const()) {
            //    fent.set_const();
            //}
        }
    }
}
