//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_MULTIMETHOD_HPP
#define SONIA_UTILITY_MULTIMETHOD_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
//#include <boost/mpl/fold.hpp>
//#include <boost/mpl/push_back.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>

#include "sonia/prime_config.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/function.hpp"
#include "sonia/array_view.hpp"
#include "sonia/utility/type_info.hpp"
#include "sonia/utility/polymorphic_traits.hpp"

namespace sonia {

class multimethod : public polymorphic_movable {
public:
    virtual ~multimethod() {}
};

namespace services {

SONIA_PRIME_API void register_multimethod(multimethod &&, type_info const& id, array_view<const type_info> fullid);
SONIA_PRIME_API multimethod const* get_multimethod(type_info const& msig);

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

struct type_filler {
    type_info * it_;
    type_filler(type_info * it) : it_(it) {}
    template <typename U> void operator()(U) { *it_++ = typeid(U); }
};

template <typename MethodIDT, typename SigT>
struct multimethod_sig {
    typedef typename boost::function_types::result_type<SigT>::type result_type;
    typedef typename boost::function_types::parameter_types<SigT>::type params_mpl_seq_type;
    static const size_t params_count = boost::mpl::size<params_mpl_seq_type>::value;

    //typedef typename boost::mpl::fold<
    //    boost::mpl::vector2<MethodIDT, result_type>,
    //    params_mpl_seq_type,
    //    boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
    //>::type full_sig_vec_type;
    typedef boost::mpl::vector2<MethodIDT, SigT> full_sig_vec_type;

    // we need identity wrappers to preserve cv modifiers and references on types
    type_info arr[2 + params_count] = {typeid(MethodIDT), typeid(identity<result_type>)};

    multimethod_sig() {
        boost::mpl::for_each<params_mpl_seq_type, make_identity<boost::mpl::_>>(type_filler(arr + 2));
    }

    array_view<const type_info> params() const { return array_view<const type_info>(arr); }
};

}

template <typename MethodIDT, typename SigT>
void register_multimethod(function<SigT> const& f) {
    typedef multimethod_detail::multimethod_sig<MethodIDT, SigT> sig_t;

    sig_t mmsig;
    
    sonia::services::register_multimethod(
        concrete_multimethod<SigT>(f),
        typeid(typename sig_t::full_sig_vec_type),
        mmsig.params()
    );
}

template <typename MethodIDT, typename SigT>
function<SigT> const* get_multimethod() {
    typedef multimethod_detail::multimethod_sig<MethodIDT, SigT> sig_t;
    //sig_t mmsig;

    multimethod const* mm = sonia::services::get_multimethod(typeid(typename sig_t::full_sig_vec_type));

    return mm ? &static_cast<concrete_multimethod<SigT> const&>(*mm)() : nullptr;
}

//template<TupleT>
//multimethod_apply()

}

#endif // SONIA_UTILITY_MULTIMETHOD_HPP
