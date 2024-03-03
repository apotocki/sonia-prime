//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "attr_parser.hpp"
#include "sonia/exceptions.hpp"

namespace sonia::xmlbuilder {

std::tuple<blob_result, func_type> attr_parser::parse(string_view val) const
{
    if (val.starts_with("{") && val.ends_with("}")) {
        string_view code = val.substr(1, val.size() - 2);
        func_type ft = func_type::unspecified;
        if (code.starts_with("{") && code.ends_with("}")) {
            code = code.substr(1, code.size() - 2);
            ft = func_type::no_return;
        }
        return { string_blob_result(code, blob_type::function), ft };
    }
    return { nil_blob_result(), func_type::unspecified };
}

std::tuple<blob_result, func_type> functional_attr_parser::parse(string_view value) const
{
    if (auto r = attr_parser::parse(value); std::get<0>(r).type == blob_type::function) {
        return { std::get<0>(r), std::get<1>(r) | func_type::is_functor };
    }
    return { string_blob_result(value, blob_type::function), func_type::no_return | func_type::is_functor };
    //throw exception("can't parse attribute value '%1%' as a function body"_fmt % value);
}

std::tuple<blob_result, func_type> particular_attr_parser<bool>::parse(string_view value) const
{
    if (auto r = attr_parser::parse(value); std::get<0>(r).type == blob_type::function) {
        return r;
    }
    if (value == "true" || value == "1") return { bool_blob_result(true), func_type::unspecified };
    if (value == "false" || value == "0") return { bool_blob_result(false), func_type::unspecified };
    throw exception("can't parse attribute value '%1%' as bool"_fmt % value);
}

}
