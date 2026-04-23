//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <functional>
#include <type_traits>

#include "sonia/shared_ptr.hpp"
#include "sonia/utility/invocation/invocation.hpp"

namespace sonia::invocation {

// Wraps any callable object (functor, lambda, std::function) as an invocation::callable.
// F must be invocable with a single bool argument.
template <typename F>
class functor_callable : public callable
{
public:
    explicit functor_callable(F fn) : fn_{ std::move(fn) } {}

    smart_blob invoke(span<const blob_result> args) override
    {
        fn_(from_blob<bool>{}(args[0]));
        return {};
    }

private:
    F fn_;
};

template <typename F>
shared_ptr<callable> make_callable(F&& fn)
{
    return make_shared<functor_callable<std::decay_t<F>>>(std::forward<F>(fn));
}

}
