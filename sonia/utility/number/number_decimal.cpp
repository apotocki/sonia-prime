//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "decimal.hpp"
#include "basic_decimal.ipp"

namespace sonia {

template class basic_decimal<
    boost::multiprecision::number<boost::multiprecision::cpp_int_backend<64, 0>>,
    int32_t
>;

}
