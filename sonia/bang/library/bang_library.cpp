//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "library.hpp"
#include "sonia/logger/logger.hpp"
#include "sonia/mp/basic_integer.hpp"
#include "sonia/mp/basic_decimal.hpp"

#include <sstream>

namespace sonia::lang::bang {

void bang_assert(vm::context& ctx)
{
    bool r = ctx.stack_back().as<bool>();
    ctx.stack_pop();
    if (!r) {
        throw exception("bang assertion error");
    }
}

void bang_any_equal(vm::context& ctx)
{
    bool result = ctx.stack_back() == ctx.stack_back(1);
    ctx.stack_pop();
    ctx.stack_back().replace(smart_blob{ bool_blob_result(result) });
}

void bang_decimal_equal(vm::context& ctx)
{
    mp::decimal r = ctx.stack_back().as<mp::decimal>();
    mp::decimal l = ctx.stack_back(1).as<mp::decimal>();
    ctx.stack_pop();
    ctx.stack_back().replace(smart_blob{ bool_blob_result(l == r) });
}

void bang_tostring(vm::context & ctx)
{
    std::ostringstream res;
    print_to_stream(res, *ctx.stack_back(), false);
    ctx.stack_pop(1);
    smart_blob r{ string_blob_result(res.str()) };
    r.allocate();
    ctx.stack_push(std::move(r));
}

void bang_print_string(vm::context& ctx)
{
    size_t argcount = ctx.stack_back().as<size_t>();
    unit & u = ctx.get_unit();
    for (size_t i = argcount; i > 0; --i) {
        u.write_cout(ctx.stack_back(i).as<string_view>());
    }
    ctx.stack_pop(argcount + 1);
}

void bang_negate(vm::context& ctx)
{
    auto val = *ctx.stack_back();
    while (val.type == blob_type::blob_reference) {
        val = *data_of<blob_result>(val);
    }
    val = blob_type_selector(val, [](auto ident, blob_result const& b) {
        using type = typename decltype(ident)::type;
        if (!is_array(b)) {
            if constexpr (std::is_same_v<type, bool>) { return bool_blob_result(!b.bp.i8value); }
            else if constexpr (std::is_integral_v<type> || std::is_same_v<type, sonia::mp::basic_integer_view<invocation_bigint_limb_type>>) { return bool_blob_result(!as<type>(b)); }
            else {
                throw exception("cna't negate value: %1%"_fmt % b);
            }
        } else if (contains_string(b)) {
            auto sv = as<std::string_view>(b);
            return bool_blob_result(sv.empty());
        } else {
            throw exception("cna't negate value: %1%"_fmt % b);
        }
    });
    ctx.stack_pop(1);
    ctx.stack_push(val);
}

void bang_concat_string(vm::context& ctx)
{
    auto l = ctx.stack_back(1).as<string_view>();
    auto r = ctx.stack_back().as<string_view>();
    auto res = make_blob_result(blob_type::string, nullptr, static_cast<uint32_t>(l.size() + r.size()));
    blob_result_allocate(&res);
    char * buff = mutable_data_of<char>(res);
    std::memcpy(buff, l.data(), l.size());
    std::memcpy(buff + l.size(), r.data(), r.size());
    ctx.stack_pop();
    ctx.stack_back().replace( smart_blob{ std::move(res) } );
}

void bang_operator_plus_integer(vm::context& ctx)
{
    auto l = ctx.stack_back(1).as<mp::integer>();
    auto r = ctx.stack_back().as<mp::integer_view>();
    auto sum = l + r;
    smart_blob res{ bigint_blob_result(sum) };
    res.allocate();

    ctx.stack_pop();
    ctx.stack_back().replace(std::move(res));
}

void bang_operator_plus_decimal(vm::context& ctx)
{
    auto l = ctx.stack_back(1).as<mp::decimal>();
    auto r = ctx.stack_back().as<mp::decimal_view>();
    auto sum = l + r;
    smart_blob res{ decimal_blob_result(sum) };
    res.allocate();
    
    ctx.stack_pop();
    ctx.stack_back().replace(std::move(res));
}

void bang_str2dec(vm::context& ctx)
{
    auto str = ctx.stack_back().as<string_view>();
    try {
        mp::decimal d{ str };
        smart_blob res{ decimal_blob_result(d) };
        res.allocate();
        ctx.stack_back().replace(std::move(res));
    } catch (std::exception const& e) {
        ctx.stack_back().replace(error_blob_result(e.what())); // nil_blob_result()
        ctx.stack_back().allocate();
    }
}

void bang_int2dec(vm::context& ctx)
{
    auto ival = ctx.stack_back().as<mp::integer_view>();
    mp::decimal dval{ ival };
    smart_blob res{ decimal_blob_result(dval) };
    res.allocate();
    ctx.stack_back().replace(std::move(res));
}

void bang_create_extern_object(vm::context& ctx)
{
    string_view name = ctx.stack_back().as<string_view>();
    if (name.starts_with("::"sv)) {
        name = name.substr(2);
        name = name.substr(2);
    }

    smart_blob resobj = ctx.env().invoke("create"sv, { string_blob_result(ctx.camel2kebab(name)) });
    if (resobj->type == blob_type::error) {
        throw exception(resobj.as<std::string>());
    }
    ctx.stack_back().replace(std::move(resobj));
}

// (obj, propName, value)->obj
void bang_set_object_property(vm::context& ctx)
{
    using namespace sonia::invocation;
    shared_ptr<invocable> obj = ctx.stack_back(2).as<wrapper_object<shared_ptr<invocable>>>().value;
    string_view prop_name = ctx.stack_back(1).as<string_view>();
    obj->set_property(ctx.camel2kebab(prop_name), *ctx.stack_back());
    ctx.stack_pop(2);
}

}
