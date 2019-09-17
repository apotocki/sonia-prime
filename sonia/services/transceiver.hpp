//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVIVES_TRANSCEIVER_HPP
#define SONIA_SERVIVES_TRANSCEIVER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <boost/mpl/at.hpp>

#include "sonia/string.hpp"
#include "sonia/optional.hpp"
#include "sonia/exceptions.hpp"

#include "sonia/utility/automatic.hpp"
#include "sonia/utility/type_durable_id.hpp"
#include "sonia/utility/bind.hpp"
#include "sonia/utility/multimethod.hpp"
#include "sonia/utility/marshaling/marshaling.hpp"
#include "sonia/utility/serialization/serialization.hpp"
#include "sonia/utility/serialization/serializable.hpp"
#include "sonia/utility/serialization/type_durable_id.hpp"
#include "sonia/utility/serialization/tuple.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"

namespace sonia::services {

class serializable_proxy_object : public serializable {};

class transceiver
{
public:
    virtual ~transceiver() = default;

    virtual void transmit_and_receive(string_view dest, serializable_proxy_object & obj) = 0;
/*
    // send and forget
    virtual void async_send(string_view dest, command<void> &) = 0;

    template <typename RT>
    boost::fibers::future<RT> async_invoke(string_view dest, command_base<RT> & cmd) {
        optional<boost::fibers::future<RT>> f;
        invoke_raw(dest, cmd, &f);
        return std::move(*f);
    }

    template <typename RT>
    RT invoke(string_view dest, command_base<RT> & cmd) {
        return async_invoke(dest, cmd).get();
    }

protected:
    virtual void invoke_raw(string_view dest, abstract_command_base & cmd, void * p_opt_future) = 0;
*/
};

template <typename RT>
class proxy_object_base
{
public:
    RT get() const { return std::move(*ret_); }

protected:
    ~proxy_object_base() {}

    optional<RT> ret_;
};

template <>
class proxy_object_base<void>
{
public:
    void get() const {}
};


template <class BindingTagT, typename ... ArgsT>
class proxy_object 
    : public proxy_object_base<typename BindingTagT::result_type>
    , public serializable_proxy_object
{
    using result_type = typename BindingTagT::result_type;
    using base_type = proxy_object_base<result_type>;

public:
    explicit proxy_object(ArgsT&& ... args)
        : proxy_tuple_{std::forward<ArgsT>(args) ...}
    {
        // check that provided arguments match the function signature
        static_assert(is_same_v<
            result_type,
            decltype(apply_placeholders(std::declval<typename BindingTagT::f_type>(), proxy_tuple_))
        >);
    }

    void serialize(range_write_iterator wit) const override
    {
        range_dereferencing_iterator rit{std::move(wit)};
        rit = encode<sonia::serialization::compressed_t>(sonia::type::durable_id::get<BindingTagT>(), std::move(rit));
        encode<sonia::serialization::compressed_t>(proxy_tuple_, std::move(rit)).flush();
    }

    void deserialize(range_read_iterator rit) override
    {
        range_dereferencing_iterator rdit{std::move(rit)};

        std::string errstr;
        rdit = decode<sonia::serialization::compressed_t>(std::move(rdit), errstr);
        if (!errstr.empty()) {
            throw exception(std::move(errstr));
        }

        if constexpr (!is_void_v<result_type>) {
            base_type::ret_.emplace();
            rdit = decode<sonia::serialization::compressed_t>(std::move(rdit), *base_type::ret_);
        }
  
        auto tv = sonia::mpl::make_transform_view<typename BindingTagT::result_transformer_t>(proxy_tuple_);
        decode<sonia::serialization::compressed_t>(std::move(rdit), tv);
    }

private:
    bind_tuple_t<ArgsT...> proxy_tuple_;
};

struct process_taged_object{};
using stub_read_iterator = range_dereferencing_iterator<serializable::range_read_iterator>;
using stub_write_iterator = range_dereferencing_iterator<serializable::range_write_iterator>;

template <typename RangeReadIteratorT, typename RangeWriteIteratorT>
void deserialize_object(RangeReadIteratorT rit, RangeWriteIteratorT writ)
{
    using read_iterator = range_dereferencing_iterator<RangeReadIteratorT>;

    read_iterator it = range_dereferencing_iterator(rit);
    automatic<sonia::type::durable_id> btid(in_place_decode<sonia::serialization::compressed_t>(it));
    
    auto * pfunc = sonia::get_multimethod<process_taged_object, void(read_iterator, RangeWriteIteratorT)>({btid->ti()});
    if (!pfunc) {
        throw exception("no tagged object processor is registered for (%1%, %2%, %3%)"_fmt % btid % typeid(read_iterator).name() % typeid(RangeWriteIteratorT).name());
    }
}

template <class BindingTagT, typename ... ArgsT>
typename BindingTagT::result_type transmit_end_receive(shared_ptr<transceiver> t, string_view dest, ArgsT&& ... args)
{
    proxy_object<BindingTagT, ArgsT...> po{std::forward<ArgsT>(args) ...};
    t->transmit_and_receive(dest, po);
    return po.get();
}

//void(stub_read_iterator, serializable::range_write_iterator)
template <class BindingTagT>
void register_transceiver_invoker()
{
    sonia::register_multimethod<process_taged_object>(function<void(stub_read_iterator&, stub_write_iterator&)>(
        [](stub_read_iterator & rit, stub_write_iterator & wit)
        {
            BindingTagT::template stub_invoke_and_encode<sonia::serialization::compressed_t>(rit, wit);
        }
    ), {typeidx(BindingTagT)});
}

}

#endif // SONIA_SERVIVES_TRANSCEIVER_HPP
