//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "attr_parser.hpp"
#include "sonia/exceptions.hpp"

#include <boost/lexical_cast.hpp>

#include <boost/algorithm/string.hpp>

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

namespace sonia::xmlbuilder {

template <typename T>
std::tuple<blob_result, std::string, bool> particular_attr_parser<T>::parse(string_view value) const
{
    if (auto r = attr_parser::parse(value); !std::get<1>(r).empty()) return r;
    if constexpr (is_string_v<T> || is_same_v<T, blob_result>) {
        auto br = particular_blob_result(value);
        blob_result_allocate(&br);
        return { br, {}, false };
    } else {
        try {
            return { particular_blob_result(boost::lexical_cast<T>(value)), {}, false};
        } catch (boost::bad_lexical_cast const&) {
            throw exception("can't parse attribute value '%1%' as '%2%'"_fmt % value % typeid(T).name());
        }
    }
    //throw exception("no attribute parser defined for type '%1%' to parse value: '%2%'"_fmt % typeid(T).name() % name);
}

template <typename T, size_t SzV>
std::tuple<blob_result, std::string, bool> particular_attr_parser<std::array<T, SzV>>::parse(string_view value) const
{
    if (auto r = attr_parser::parse(value); !std::get<1>(r).empty()) return r;
    std::vector<intermediate_element_t> result;
    result.resize(SzV);
    auto rng = std::views::split(value, " "sv);
    auto it = rng.begin(), eit = rng.end();
    for (auto elem_it = result.begin(); elem_it != result.end(); ++elem_it) {
        if (it != eit) {
            string_view value{ (*it).begin(), (*it).end() }; ++it;
            smart_blob elem{ std::get<0>(particular_attr_parser<T>().parse(value)) };
            if constexpr (is_simple_v) {
                *elem_it = from_blob<T>{}(*elem);
            }
            else {
                *elem_it = elem.detach();
            }
        }
        else if (elem_it != result.begin()) {
            *elem_it = *(elem_it - 1);
        }
    }

    blob_result r = array_blob_result(std::span{ result });
    blob_result_allocate(&r);
    return { r , {}, false };
}

template <typename T>
std::tuple<blob_result, std::string, bool> particular_attr_parser<std::vector<T>>::parse(string_view value) const
{
    if (auto r = attr_parser::parse(value); !std::get<1>(r).empty()) return r;
    std::vector<intermediate_element_t> result;
    auto rng = std::views::split(value, " "sv);
    for (auto it = rng.begin(), eit = rng.end(); it != eit; ++it) {
        string_view value{ (*it).begin(), (*it).end() }; 
        smart_blob elem{ std::get<0>(particular_attr_parser<T>().parse(value)) };
        if constexpr (is_simple_v) {
            result.push_back(from_blob<T>{}(*elem));
        } else {
            result.push_back(elem.detach());
        }
    }
    if (result.empty()) {
        return { nil_blob_result(), {}, false };
    } else if (result.size() == 1) {
        return { particular_blob_result(result[0]), {}, false };
    }
    blob_result r = array_blob_result(std::span{ result });
    blob_result_allocate(&r);
    return { r , {}, false };
}

template <typename ... Ts>
std::tuple<blob_result, std::string, bool> particular_attr_parser<std::tuple<Ts ...>>::parse(string_view value) const
{
    if (auto r = attr_parser::parse(value); !std::get<1>(r).empty()) return r;
    std::vector<blob_result> result;
    do_parse(std::make_index_sequence<sizeof... (Ts)>(), std::views::split(value, " "sv), result);
    blob_result r = array_blob_result(std::span{ result });
    blob_result_allocate(&r);
    return { r, {}, false };
}

}
