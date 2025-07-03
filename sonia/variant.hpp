//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <type_traits>

#include <boost/variant.hpp>
#include <boost/variant/multivisitors.hpp>

#include "sonia/utility/functional/equal.hpp"

namespace sonia {

using boost::variant;
using boost::recursive_variant_;
using boost::static_visitor;
using boost::apply_visitor;
using boost::get;
using boost::make_recursive_variant;
using boost::recursive_wrapper;

template <typename RT, typename FT>
class functional_visitor : public static_visitor<RT>
{
    FT fnl_;

public:
    template <typename F>
    explicit functional_visitor(F && arg) : fnl_{ std::forward<F>(arg) } {}

    template <typename T>
    inline RT operator()(T&& arg) const { return fnl_(std::forward<T>(arg)); }
};

template <typename RT, typename FAT>
functional_visitor<RT, std::remove_cvref_t<FAT>> make_functional_visitor(FAT&& arg)
{
    return functional_visitor<RT, std::remove_cvref_t<FAT>>{ std::forward<FAT>(arg) };
}

}

#define SONIA_VARIANT_DECLARED

#include "sonia/utility/functional/equal/variant.hpp"
#include "sonia/utility/functional/less/variant.hpp"
#include "sonia/utility/functional/hash/variant.hpp"
//#include "sonia/utility/streaming/variant.hpp"
