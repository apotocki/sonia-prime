//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/services/service.hpp"

#include "sonia/type_traits.hpp"
#include "sonia/utility/marshaling/stub_parameter.hpp"

namespace sonia {

/*
template <typename T>
requires(is_base_of_v<service, remove_cvref_t<T>>)
struct stub_bound_parameter<T>
{
    using type = stub_parameter_holder<remove_cvref_t<T>, shared_ptr<remove_cvref_t<T>>>;

    static constexpr bool is_modifiable = false;
};
*/

template <std::derived_from<service> T>
struct stub_bound_parameter<T&>
{
    using type = stub_parameter_holder<remove_cv_t<T>, shared_ptr<remove_cv_t<T>>>;

    static constexpr bool is_modifiable = false;
};

}

