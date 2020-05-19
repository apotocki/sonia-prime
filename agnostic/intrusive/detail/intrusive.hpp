// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_INTRUSIVE_DETAIL_INTRUSIVE_HPP
#define AGNOSTIC_INTRUSIVE_DETAIL_INTRUSIVE_HPP

#pragma once

namespace agnostic::intrusive {

namespace detail {

struct constant_time_size_tag {};
struct size_type_tag {};
struct hook_tag {};
struct compare_tag{};

template <class TagT, class DefaultT, class ... OptionsT> struct find_option;
template <class TagT, class DefaultT, class ... OptionsT> using find_option_t = typename find_option<TagT, DefaultT, OptionsT...>::type;

template <class TagT, class DefaultT> struct find_option<TagT, DefaultT> { using type = DefaultT; };
template <class TagT, class DefaultT, class OT0, class ... OptionsT> struct find_option<TagT, DefaultT, OT0, OptionsT...>
{
    using type = typename std::conditional_t<
        std::is_same_v<typename OT0::tag, TagT>,
        std::type_identity<OT0>,
        find_option<TagT, DefaultT, OptionsT...>
    >::type;
};

template <typename SizeT, bool EnabledV>
struct size_plugin
{
protected:
    SizeT size_;
    void increment_size() noexcept { ++size_; }
    void decrement_size() noexcept { --size_; }

public:
    SizeT size() const { return size_; }
};

template <typename SizeT>
struct size_plugin<SizeT, false>
{
    void increment_size() noexcept { }
    void decrement_size() noexcept { }
};

template <class LessT>
struct basic_compare
{
    using tag = compare_tag;
    template <typename LT, typename RT>
    int operator()(LT const& l, RT const& r) const
    {
        if (LessT()(l, r)) return -1;
        if (LessT()(r, l)) return 1;
        return 0;
    }
};

} // detail

template <class T, class HookT, HookT T::* PtrToMemberV> struct member_hook { using tag = detail::hook_tag; };
template <bool EnabledT> struct constant_time_size { using tag = detail::constant_time_size_tag; static constexpr bool value = EnabledT; };
template <typename SizeT> struct size_type { using tag = detail::size_type_tag; using type = SizeT; };

}

#endif // AGNOSTIC_INTRUSIVE_DETAIL_INTRUSIVE_HPP
