// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#pragma once

#include "../find_option.hpp"

namespace agnostic::intrusive {

namespace detail {

struct constant_time_size_tag {};
struct size_type_tag {};
struct hook_tag {};
struct compare_tag{};

template <typename SizeT, bool EnabledV>
struct size_plugin
{
protected:
    SizeT size_;
    inline void increment_size() noexcept { ++size_; }
    inline void decrement_size() noexcept { --size_; }

public:
    inline SizeT size() const noexcept { return size_; }
};

template <typename SizeT>
struct size_plugin<SizeT, false>
{
    inline void increment_size() noexcept { }
    inline void decrement_size() noexcept { }
};

template <class LessT>
struct basic_compare
{
    using tag = compare_tag;
    template <typename LT, typename RT>
    inline int operator()(LT const& l, RT const& r) const
    {
        if (LessT{}(l, r)) return -1;
        if (LessT{}(r, l)) return 1;
        return 0;
    }
};

} // detail

template <class T, class HookT, HookT T::* PtrToMemberV> struct member_hook { using tag = detail::hook_tag; };
template <bool EnabledT> struct constant_time_size { using tag = detail::constant_time_size_tag; static constexpr bool value = EnabledT; };
template <typename SizeT> struct size_type { using tag = detail::size_type_tag; using type = SizeT; };

}

