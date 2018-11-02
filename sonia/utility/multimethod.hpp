//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_MULTIMETHOD_HPP
#define SONIA_UTILITY_MULTIMETHOD_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <array>

#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>

#include "sonia/prime_config.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/function.hpp"
#include "sonia/array_view.hpp"
#include "sonia/utility/polymorphic_traits.hpp"

namespace sonia {

class multimethod : public polymorphic_movable {
public:
    virtual ~multimethod() {}
};

namespace services {

SONIA_PRIME_API void register_multimethod(multimethod &&, array_view<const std::type_index>);
SONIA_PRIME_API multimethod const* get_multimethod(array_view<const std::type_index>);

}

template <typename SigT>
class concrete_multimethod : public multimethod {
    typedef function<SigT> func_t;

public:
    template <typename ArgT>
    concrete_multimethod(ArgT && arg) : func_(std::forward<ArgT>(arg)) {}
    concrete_multimethod(concrete_multimethod const&) = default;
    concrete_multimethod(concrete_multimethod &&) = default;

    func_t const& operator()() const { return func_; }

    polymorphic_movable* move(void* address, size_t sz) {
        BOOST_ASSERT(sz >= sizeof(concrete_multimethod));
        return new(address) concrete_multimethod(std::move(*this));
    }
    
private:
    func_t func_;
};

namespace multimethod_detail {

template <typename MethodIDT, typename SigT>
struct multimethod_sig {
    typedef typename boost::function_types::result_type<SigT>::type result_type;
    typedef typename boost::function_types::parameter_types<SigT>::type params_mpl_seq_type;
    static const size_t params_count = boost::mpl::size<params_mpl_seq_type>::value;
    typedef boost::mpl::vector2<MethodIDT, SigT> type;

    static std::type_index ti() { return typeid(type); }
};

}

template <typename MethodIDT, typename ... TS, typename SigT>
void register_multimethod(function<SigT> const& f)
{
    typedef multimethod_detail::multimethod_sig<MethodIDT, SigT> sig_t;
    std::array<std::type_index, 1 + sizeof ...(TS)> tis{sig_t::ti(), typeid(TS) ...};
    sonia::services::register_multimethod(concrete_multimethod<SigT>(f), to_array_view(tis));
}

template <typename MethodIDT, typename SigT, typename ... TS>
function<SigT> const* get_multimethod()
{
    typedef multimethod_detail::multimethod_sig<MethodIDT, SigT> sig_t;
    std::array<std::type_index, 1 + sizeof ...(TS)> tis{sig_t::ti(), typeid(TS) ...};
    multimethod const* mm = sonia::services::get_multimethod(to_array_view(tis));
    return mm ? &static_cast<concrete_multimethod<SigT> const&>(*mm)() : nullptr;
}

template <typename MethodIDT, typename SigT, size_t ... I, class ArrT>
function<SigT> const* get_multimethod(std::index_sequence<I ...>, ArrT const& arr)
{
    typedef multimethod_detail::multimethod_sig<MethodIDT, SigT> sig_t;
    std::array<std::type_index, 1 + sizeof ...(I)> tis{sig_t::ti(), arr[I] ...};
    multimethod const* mm = sonia::services::get_multimethod(to_array_view(tis));
    return mm ? &static_cast<concrete_multimethod<SigT> const&>(*mm)() : nullptr;
}

template <typename MethodIDT, typename SigT, size_t N>
function<SigT> const* get_multimethod(const std::type_index (&tis_)[N])
{
    return get_multimethod<MethodIDT, SigT>(std::make_index_sequence<N>(), tis_);
}

template <typename MethodIDT, typename SigT, size_t N>
function<SigT> const* get_multimethod(const std::type_info (&tis_)[N])
{
    return get_multimethod<MethodIDT, SigT>(std::make_index_sequence<N>(), tis_);
}

//template <typename MethodIDT, typename SigT, size_t N>
//function<SigT> const* get_multimethod(std::initializer_list<std::type_index> il)
//{
//    return nullptr;
//}
// std::array<std::type_index, N>
//template <typename MethodIDT, typename SigT>
//function<SigT> const* get_multimethod(std::initializer_list<std::type_index> l)
//{
//    static const size_t ilsz = l.size();
//    std::array<std::type_index, ilsz> arr = {typeid(int)};
//    return get_multimethod2<MethodIDT, SigT>(std::make_index_sequence<ilsz>(), arr);
//    /*
//    typedef multimethod_detail::multimethod_sig<MethodIDT, SigT> sig_t;
//
//    std::make_index_sequence<N>()
//
//    std::array<std::type_index, 1 + l.size()> tis{sig_t::ti(), tis ...};
//
//    multimethod const* mm = sonia::services::get_multimethod(to_array_view(tis));
//
//    return mm ? &static_cast<concrete_multimethod<SigT> const&>(*mm)() : nullptr;
//    */
//}

//template<TupleT>
//multimethod_apply()

}

#endif // SONIA_UTILITY_MULTIMETHOD_HPP
