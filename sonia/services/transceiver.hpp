//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVIVES_TRANSCEIVER_HPP
#define SONIA_SERVIVES_TRANSCEIVER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

#include "sonia/string.hpp"
#include "sonia/optional.hpp"
#include "sonia/utility/automatic.hpp"
#include "sonia/utility/type_durable_id.hpp"
#include "sonia/utility/bind.hpp"
#include "sonia/utility/multimethod.hpp"
#include "sonia/utility/serialization/serialization.hpp"
#include "sonia/utility/serialization/serializable.hpp"
#include "sonia/utility/serialization/type_durable_id.hpp"
#include "sonia/utility/serialization/tuple.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"

//#include "sonia/utility/command.hpp"

//#include <boost/fiber/future.hpp>

namespace sonia { namespace services {

class transceiver {
public:
    virtual ~transceiver() {}

    virtual void transmit_and_receive(string_view dest, sonia::type::durable_id objid, serializable & obj) = 0;
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
class proxy_object_base {
public:
    RT get() const { return std::move(*ret_); }

protected:
    ~proxy_object_base() {}

    optional<RT> ret_;
};

template <>
class proxy_object_base<void> {
public:
    void get() const {}
};

template <class BindingTagT, typename ... ArgsT>
class proxy_object 
    : public proxy_object_base<typename BindingTagT::result_type>
    , public serializable
{
    using result_type = typename BindingTagT::result_type;

public:
    explicit proxy_object(ArgsT&& ... args)
        : proxy_tuple_(std::forward<ArgsT>(args) ...)
    {
        // check that provided arguments match the function signature
        static_assert(is_same_v<
            result_type,
            decltype(apply_placeholders(std::declval<typename BindingTagT::f_type>(), proxy_tuple_))
        >);
    }

    void serialize(range_write_iterator wit) const override
    {
        auto enc = make_encoder<sonia::serialization::compressed_t>(
            range_dereferencing_iterator(wit)
        );
        enc & sonia::type::durable_id::get<BindingTagT>() & proxy_tuple_;
        enc.iterator().flush();
    }

    void deserialize(range_read_iterator rit) const override
    {
        auto it = range_dereferencing_iterator(rit);
        throw not_implemented_error();
    }

private:
    bind_tuple_t<ArgsT...> proxy_tuple_;
};

struct process_taged_object{};

template <typename RangeReadIteratorT, typename RangeWriteIteratorT>
void deserialize_object(RangeReadIteratorT rit, RangeWriteIteratorT writ)
{
    using read_iterator = range_dereferencing_iterator<RangeReadIteratorT>;
    //using write_iterator = range_dereferencing_iterator<RangeWriteIteratorT>;

    read_iterator it = range_dereferencing_iterator(rit);
    automatic<sonia::type::durable_id> btid(in_place_decode<sonia::serialization::compressed_t>(it));
    
    auto * pfunc = sonia::get_multimethod<process_taged_object, void(read_iterator, RangeWriteIteratorT)>({btid->ti()});
    if (!pfunc) {
        throw exception("no tagged object processor is registered for (%1%, %2%, %3%)"_fmt % btid % typeid(read_iterator).name() % typeid(RangeWriteIteratorT).name());
    }
}

template <class BindingTagT>
class stub_object : public serializable {
public:

    void deserialize(range_read_iterator rit) const override
    {

    }

private:
    typename BindingTagT::stub_tuple_t stub_tuple_;
};

template <typename BindingTagT, typename ... ArgsT>
typename BindingTagT::result_type transmit_end_receive(shared_ptr<transceiver> t, string_view dest, ArgsT&& ... args)
{
    proxy_object<BindingTagT, ArgsT...> po(std::forward<ArgsT>(args) ...);
    t->transmit_and_receive(
        dest,
        sonia::type::durable_id::get<BindingTagT>(),
        po);
    return po.get();
}

}}

#endif // SONIA_SERVIVES_TRANSCEIVER_HPP
