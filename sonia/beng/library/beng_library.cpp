//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "library.hpp"
#include "sonia/logger/logger.hpp"

#include <sstream>

namespace sonia::lang::beng {

void beng_tostring(vm::context & ctx)
{
    std::ostringstream res;
    res << ctx.stack_back();
    ctx.stack_pop(1);
    smart_blob r{ string_blob_result(res.str()) };
    r.allocate();
    ctx.stack_push(std::move(r));
}

void beng_print_string(vm::context& ctx)
{
    string_view str = ctx.stack_back().as<string_view>();
    GLOBAL_LOG_INFO() << str;
    ctx.stack_pop();
}

}
