//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "types.hpp"

#include <cstdlib>
#include <boost/algorithm/string/trim_all.hpp>

#if __APPLE__ || __ANDROID__
namespace std::views {

template <typename RangeT>
std::vector<RangeT> split(RangeT rng, RangeT delim)
{
    std::vector<RangeT> result;
    boost::split(result, rng, boost::is_any_of(delim));
    return result;
}

}
#else
#   include <ranges>
#endif

namespace sonia::xmlbuilder::detail {

template <typename BlobT, typename XT>
array_value_parser_visitor::result_type array_value_parser_visitor::do_apply(XT const& xv) const
{
    if (value.empty()) {
        return { (size ? optional<blob_result>{} : array_blob_result(span<BlobT>{})), value };
    }
    auto charit = value.begin();
    char fchar = *charit;
    if (fchar == '[') {
        fchar = ']';
        ++charit;
    } else if (fchar == '(') {
        fchar = ')';
        ++charit;
    } else {
        fchar = 0;
    }
    boost::container::small_vector<BlobT, 16> buff;
    buff.reserve(size);

    for (;;) {
        type_value_parser_visitor visitor{ string_view{charit, value.end()}, false };
        auto pair = visitor(xv);
        if (!pair.first) {
            return { nullopt, value };
        }
        buff.push_back(as<BlobT>(*pair.first));
        charit = pair.second.begin();
        while (charit != value.end() && isspace(*charit)) ++charit;
        if (charit == value.end()) break;
        if (fchar && *charit == fchar) {
            ++charit;
            break;
        }
        if (*charit == ',') {
            ++charit;
        }
    }
    if (buff.size() <= size) {
        blob_result res = array_blob_result(span{ buff });
        blob_result_allocate(&res);
        return { res, string_view{charit, value.end()} };
    }
    
    return { nullopt, value };
}

array_value_parser_visitor::result_type array_value_parser_visitor::operator()(x_float const& xf) const
{
    return do_apply<float>(xf);
}

type_value_parser_visitor::type_value_parser_visitor(string_view v, bool greedy) noexcept
    : value{ v }, is_greedy{ greedy }
{}

type_value_parser_visitor::result_type type_value_parser_visitor::operator()(x_bool const&) const noexcept
{
    string_view tvalue = boost::algorithm::trim_left_copy(value);
    auto checker = [tvalue](string_view pattern) -> bool {
        return tvalue.starts_with(pattern) && (
            tvalue.size() == pattern.size() ||
            (isspace(tvalue[pattern.size()]) || ispunct(tvalue[pattern.size()]))
        );
    };
    if (checker("true"sv)) return { bool_blob_result(true), tvalue.substr(4) };
    if (checker("false"sv)) return { bool_blob_result(false), tvalue.substr(5) };
    if (!tvalue.empty() && tvalue.front() == '0' || tvalue.front() == '1') {
        boost::container::small_vector<char, 16> buff{ tvalue.begin(), tvalue.end() };
        buff.push_back(0);
        char* estring;
        auto val = strtoull(buff.data(), &estring, 10);
        if (estring != buff.data()) {
            return { bool_blob_result(!!val), tvalue.substr(estring - buff.data()) };
        }
    }
    return { nullopt, value };
}

type_value_parser_visitor::result_type type_value_parser_visitor::operator()(x_float const&) const noexcept
{
    string_view tvalue = boost::algorithm::trim_left_copy(value);
    boost::container::small_vector<char, 16> buff{ tvalue.begin(), tvalue.end() };
    buff.push_back(0);
    char* estring;
    float fvalue = strtof(buff.data(), &estring);
    if (estring == buff.data()) [[unlikely]] {
        return { nullopt, value };
    }
    return { f32_blob_result(fvalue), tvalue.substr(estring - buff.data()) };
}

type_value_parser_visitor::result_type type_value_parser_visitor::operator()(x_string const&) const noexcept
{
    if (is_greedy) return { string_blob_result(value), ""sv };
    string_view tvalue = boost::algorithm::trim_left_copy(value);
    if (tvalue.empty()) return { nullopt, ""s }; // if not greedy empty string must be explicitly defined by qoutes
    auto charit = tvalue.begin();
    char fchar = *charit++;
    if (fchar == '"' || fchar == '\'') {
        for (auto it = charit; it != tvalue.end(); ++it) {
            if (*it == fchar) [[unlikely]] {
                string_view rstr = tvalue.substr(1, size_t(it - tvalue.end() - 1));
                return { string_blob_result(rstr), tvalue.substr(rstr.size() + 2) };
            } else if (*it == '\\') [[unlikely]] {
                ++it;
                if (it == tvalue.end()) break;
            }
        }
    } else if (std::ispunct(fchar)) {
        return { string_blob_result(""s), tvalue };
    }
    // skip until space/punct
    while (charit != tvalue.end() && !std::isspace(*charit) && !std::ispunct(*charit)) ++charit;
    string_view rstr = tvalue.substr(0, size_t(charit - tvalue.end()));
    return { string_blob_result(rstr), tvalue.substr(rstr.size()) };
}

type_value_parser_visitor::result_type type_value_parser_visitor::operator()(x_action const&) const noexcept
{
    // to do: if a function is a element of array or tuple
    string_view tvalue = boost::algorithm::trim_copy(value);
    return { string_blob_result(tvalue, blob_type::function), ""sv };
}

type_value_parser_visitor::result_type type_value_parser_visitor::operator()(x_array<x_generic_type> const& at) const noexcept
{
    string_view tvalue = boost::algorithm::trim_left_copy(value);
    return apply_visitor(array_value_parser_visitor{ tvalue, at.size }, at.type);
}

}
