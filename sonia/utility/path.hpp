//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <string_view>
#include "sonia/exceptions.hpp"

namespace sonia::path {

// filename without extension
template<typename CharT, typename TraitsT>
std::basic_string_view<CharT, TraitsT> stem(std::basic_string_view<CharT, TraitsT> p)
{
    const CharT *pext = nullptr;
    for (auto rit = p.rbegin(), erit = p.rend(); rit != erit; ++rit) {
        CharT c = *rit;
        if (c == '.' && !pext) {
            pext = &*rit;
        } else if (c == '/') {
            break;
            //THROW_NOT_IMPLEMENTED_ERROR();
            //return p.substr(rit.base() - p.begin(), pext? std::string::npos : rit - p.rend());
        }
    }
    THROW_NOT_IMPLEMENTED_ERROR();
}

template<typename CharT, typename TraitsT>
std::basic_string_view<CharT, TraitsT> extension(std::basic_string_view<CharT, TraitsT> p)
{
    for (auto rit = p.rbegin(), erit = p.rend(); rit != erit; ++rit) {
        CharT c = *rit;
        if (c == '.') {
            return p.substr(rit.base() - p.begin());
        } else if (c == '/') {
            break;
        }
    }
    return p.substr(p.size());
}

template<typename CharT, typename TraitsT>
std::basic_string_view<CharT, TraitsT> parent_path(std::basic_string_view<CharT, TraitsT> p)
{
    for (auto rit = p.rbegin(), erit = p.rend(); rit != erit; ++rit) {
        CharT c = *rit;
        if (c == '/') {
            return p.substr(0, rit.base() - p.begin() - 1);
        }
    }
    return p.substr(0, 0);
}

template<typename CharT, typename TraitsT>
std::basic_string_view<CharT, TraitsT> filename(std::basic_string_view<CharT, TraitsT> p)
{
    for (auto rit = p.rbegin(), erit = p.rend(); rit != erit; ++rit) {
        CharT c = *rit;
        if (c == '/') {
            return p.substr(rit.base() - p.begin());
        }
    }
    return p;
}

}
