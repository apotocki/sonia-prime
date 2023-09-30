//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "attr_parser.hpp"
#include "sonia/exceptions.hpp"

namespace sonia::xmlbuilder {

std::tuple<blob_result, std::string, bool> attr_parser::parse(string_view val) const
{
    if (val.starts_with("{") && val.ends_with("}")) {
        string_view code = val.substr(1, val.size() - 2);
        bool no_return;
        if (code.starts_with("{") && code.ends_with("}")) {
            code = code.substr(1, code.size() - 2);
            no_return = true;
        }
        else {
            no_return = false;
        }
        return { nil_blob_result(), std::string{code}, no_return };
    }
    return { nil_blob_result(), {}, false };
}

std::tuple<blob_result, std::string, bool> functional_attr_parser::parse(string_view value) const
{
    if (auto r = attr_parser::parse(value); !std::get<1>(r).empty()) {
        return { nil_blob_result(), std::get<1>(r), true };
    }
    throw exception("can't parse attribute value '%1%' as a function body"_fmt % value);
}

std::tuple<blob_result, std::string, bool> particular_attr_parser<bool>::parse(string_view value) const
{
    if (auto r = attr_parser::parse(value); !std::get<1>(r).empty()) {
        return r;
    }
    if (value == "true" || value == "1") return { bool_blob_result(true), {}, false };
    if (value == "false" || value == "0") return { bool_blob_result(false), {}, false };
    throw exception("can't parse attribute value '%1%' as bool"_fmt % value);
}

}
