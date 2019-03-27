//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_MARSHALING_STUB_PARAMETER_SERVICE_HPP
#define SONIA_UTILITY_MARSHALING_STUB_PARAMETER_SERVICE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

#include "sonia/services/service.hpp"

#include "sonia/type_traits.hpp"
#include "sonia/utility/marshaling/stub_parameter.hpp"

namespace sonia {

template <typename T>
struct stub_bound_parameter<T, enable_if_t<is_base_of_v<service, remove_cvref_t<T>>>>
{
    using type = stub_parameter_holder<remove_cvref_t<T>, shared_ptr<remove_cvref_t<T>>>;

    static constexpr bool is_modifiable = false;
};

}

#endif // SONIA_UTILITY_MARSHALING_STUB_PARAMETER_SERVICE_HPP
