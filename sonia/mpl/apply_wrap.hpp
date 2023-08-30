//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

namespace sonia::mpl {

template <typename F, typename ... ArgsT>
struct apply_wrap
{
    using type = typename F::template apply<ArgsT...>::type;
};

template <typename F, typename ... ArgsT> using apply_wrap_t = typename apply_wrap<F, ArgsT...>::type;

}
