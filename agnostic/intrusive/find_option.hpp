// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#include "agnostic/std/type_traits/conditional.hpp"
#include "agnostic/std/type_traits/type_identity.hpp"
#include "agnostic/std/type_traits/is_same.hpp"

namespace agnostic::intrusive {

template <typename T> concept Option = requires { typename T::tag; };

template <class TagT, class DefaultT, Option ... OptionsT> struct find_option;
template <class TagT, class DefaultT, Option ... OptionsT> using find_option_t = typename find_option<TagT, DefaultT, OptionsT...>::type;

template <class TagT, class DefaultT> struct find_option<TagT, DefaultT> { using type = DefaultT; };
template <class TagT, class DefaultT, class OT0, Option ... OptionsT> struct find_option<TagT, DefaultT, OT0, OptionsT...>
{
    using type = typename std::conditional_t<
        std::is_same_v<typename OT0::tag, TagT>,
        std::type_identity<OT0>,
        find_option<TagT, DefaultT, OptionsT...>
    >::type;
};

}
