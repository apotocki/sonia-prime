//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_POLYMORPHIC_VECTOR_HPP
#define SONIA_UTILITY_POLYMORPHIC_VECTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"

namespace sonia {

template <typename ElementT>
class polymorphic_vector
{
public:
    using value_type = ElementT;

    virtual ~polymorphic_vector() {}

    virtual value_type * begin() = 0;
    virtual value_type const* begin() const = 0;
    virtual value_type * end() = 0;
    virtual value_type const* end() = 0;
    virtual size_t size() const = 0;
    virtual array_view<value_type> to_array_view() = 0;
    virtual array_view<const value_type> to_array_view() const = 0;
    
    virtual void resize(size_t sz, value_type const& v = value_type{}) = 0;
};

template <typename VectorT>
class polymorphic_vector_adapter : polymorphic_vector<typename remove_cvref_t<VectorT>::value_type>
{
public:
    using value_type = typename polymorphic_vector_adapter::value_type;

    value_type * begin() override { return vec_.begin(); }
    value_type const* begin() const override { return std::as_const(vec_).begin(); }
    value_type * end() override { return vec_.end(); }
    value_type const* end() override { vec_.end(); }
    size_t size() const override { vec_.size(); }

    array_view<value_type> to_array_view() override
    {
        return array_view(vec_.begin(), vec_.size());
    }

    array_view<const value_type> to_array_view() const override
    {
        return array_view(std::as_const(vec_).begin(), vec_.size());
    }

    void resize(size_t sz, value_type const& v) override
    {
        vec_.resize(sz, v);
    }

private:
    VectorT vec_;
};

}

#endif // SONIA_UTILITY_POLYMORPHIC_VECTOR_HPP
