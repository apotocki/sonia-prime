//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

// impl based on https://stackoverflow.com/questions/3669833/c11-scope-exit-guard-a-good-idea

#ifndef SONIA_SCOPE_EXIT_HPP
#define SONIA_SCOPE_EXIT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

namespace sonia {

template<typename T>
class scope_exit
{
public:
    scope_exit(scope_exit const&);
    explicit scope_exit(T&& exitScope) : exitScope_(std::forward<T>(exitScope)){}
    ~scope_exit() noexcept { try { exitScope_(); } catch(...) {} }
private:
    T exitScope_;
};          

template <typename T>
scope_exit<T> scope_exit_create(T&& exitScope)
{
    return scope_exit<T>(std::forward<T>(exitScope));
}

}

#define SONIA_UTILITY_EXIT_SCOPE_LINENAME_CAT(name, line) name##line
#define SONIA_UTILITY_EXIT_SCOPE_LINENAME(name, line) SONIA_UTILITY_EXIT_SCOPE_LINENAME_CAT(name, line)
//#define SCOPE_EXIT(func) const auto& SONIA_UTILITY_EXIT_SCOPE_LINENAME(EXIT, __LINE__) = ::sonia::scope_exit_create(func)
#define SCOPE_EXIT(...) const auto& SONIA_UTILITY_EXIT_SCOPE_LINENAME(EXIT, __LINE__) = ::sonia::scope_exit_create(__VA_ARGS__)

#endif // SONIA_SCOPE_EXIT_HPP
