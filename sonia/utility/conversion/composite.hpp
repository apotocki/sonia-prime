/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <utility>

#include "sonia/type_traits.hpp"
#include "concrete_converter.hpp"

#include "tags.hpp"

namespace sonia::conversion {

template <typename ProviderConverterT, typename CustomerTagT>
class custom_converter
{
public:
    static const size_t length = ProviderConverterT::length + 1;
    typedef typename CustomerTagT::customer_tag customer_tag;
    typedef typename CustomerTagT::provider_tag customer_as_provider_tag;

    typedef concrete_converter<typename ProviderConverterT::customer_as_provider_tag, customer_tag> encoderimpl;
    typedef typename ProviderConverterT::init_tag provider_init_tag;

    typedef std::pair<provider_init_tag, CustomerTagT> init_tag;

    custom_converter(provider_init_tag const& pv, CustomerTagT const& cv)
        : itv_(pv, cv)
    { }
    
    explicit custom_converter(init_tag const& it)
        : itv_(it)
    { }

    inline static CustomerTagT const& get_consumer_tag(init_tag const& t) {
        return t.second();
    }

    inline CustomerTagT const& get_consumer_tag() const { return get_consumer_tag(itv_); }

    inline typename ProviderConverterT::customer_as_provider_tag const& get_provider_tag() const {
        return ProviderConverterT::get_consumer_tag(itv_.first());
    }

    init_tag const& get_init_tag() const {
        return itv_;
    }

    void operator= (custom_converter const& rhs) {
        itv_ = rhs.itv_;
    }

    // meta
    template <typename NextCustomerT>
    struct apply
    {
        class type : public encoderimpl::template apply<
              typename ProviderConverterT::template apply<type>::type
            , NextCustomerT>::type
        { };
    };
    
    template <typename NextCustomerTagT>
    inline custom_converter<custom_converter, NextCustomerTagT> operator >> (NextCustomerTagT const& nc)
    {
        return custom_converter<custom_converter, NextCustomerTagT>(itv_, nc);
    }
    
private:
    init_tag itv_;
};

template <typename TagT>
class null_converter
{
public:
    static const size_t length = 0;
    typedef TagT customer_as_provider_tag;
    typedef TagT init_tag;

    explicit null_converter(init_tag const& itv) : itv_(itv) {}

    template <typename NextCustomerT>
    struct apply
    {
        typedef void type;
    };

    inline static init_tag const& get_consumer_tag(init_tag const& t) {
        return t;
    }

    inline init_tag const& get_consumer_tag() const { return get_consumer_tag(itv_); }


    init_tag const& get_init_tag() const {
        return itv_;
    }

private:
    init_tag itv_;
};

// start of chain handler
template <typename ProviderTagT, typename CustomerTagT>
typename enable_if_t<is_base_of_v<cvt_tag, ProviderTagT>, custom_converter<null_converter<ProviderTagT>, CustomerTagT>>
operator >> (ProviderTagT const& p, CustomerTagT const& c)
{
    return custom_converter<null_converter<ProviderTagT>, CustomerTagT>(p, c);
}

}
