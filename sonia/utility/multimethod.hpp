//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <utility>

#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>

#include "sonia/prime_config.hpp"
#include "sonia/span.hpp"
#include "sonia/string.hpp"
#include "sonia/variant.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/function.hpp"
#include "sonia/utility/polymorphic_traits.hpp"
#include "sonia/exceptions.hpp"

namespace sonia {

class multimethod : public polymorphic_movable, public polymorphic_clonable
{
public:
    virtual ~multimethod() = default;
};

namespace services {

using mm_id_elem_t = variant<std::type_index, string_view>;

SONIA_PRIME_API void register_multimethod(multimethod &&, span<const mm_id_elem_t>);
SONIA_PRIME_API multimethod const* get_multimethod(span<const mm_id_elem_t>);
SONIA_PRIME_API void copy_multimethods(span<const mm_id_elem_t> from, span<const mm_id_elem_t> to);

inline void register_multimethod(multimethod&& mm, std::initializer_list<mm_id_elem_t> l)
{
    register_multimethod(std::move(mm), span{ l });
}

inline multimethod const* get_multimethod(std::initializer_list<mm_id_elem_t> l)
{
    return get_multimethod(span{ l });
}

inline void copy_multimethods(std::initializer_list<mm_id_elem_t> from, std::initializer_list<mm_id_elem_t> to)
{
    copy_multimethods(span{ from }, span{ to });
}

}

template <typename SigT>
class concrete_multimethod : public multimethod
{
    using func_t = function<SigT>;

public:
    template <typename ArgT>
    concrete_multimethod(in_place_t, ArgT && arg) : func_(std::forward<ArgT>(arg)) {}
    concrete_multimethod(concrete_multimethod const&) = default;
    concrete_multimethod(concrete_multimethod &&) = default;

    func_t const& operator()() const { return func_; }

    SONIA_POLYMORPHIC_MOVABLE_IMPL(concrete_multimethod);
    SONIA_POLYMORPHIC_CLONABLE_IMPL(concrete_multimethod);

private:
    func_t func_;
};

namespace multimethod_detail {

template <typename MethodIDT, typename SigT>
struct multimethod_sig
{
    using type = boost::mpl::vector2<MethodIDT, SigT>;
    using result_type = typename boost::function_types::result_type<SigT>::type;
    using params_mpl_seq_type = typename boost::function_types::parameter_types<SigT>::type;

    static constexpr size_t params_count = boost::mpl::size<params_mpl_seq_type>::value;

    static std::type_index ti() { return typeid(type); }
};

}

template <typename MethodIDT, typename ... TS, typename SigT>
void register_multimethod(function<SigT> const& f)
{
    using sig_t = multimethod_detail::multimethod_sig<MethodIDT, SigT>;
    sonia::services::register_multimethod(concrete_multimethod<SigT>(in_place, f), { sig_t::ti(), typeid(TS) ... });
}

template <typename MethodIDT, typename SigT, size_t ... I, class ArrT>
void register_multimethod(function<SigT> const& f, std::index_sequence<I ...>, ArrT const& arr)
{
    using sig_t = multimethod_detail::multimethod_sig<MethodIDT, SigT>;
    services::register_multimethod(concrete_multimethod<SigT>(in_place, f), { sig_t::ti(), arr[I] ... });
}

template <typename MethodIDT, typename SigT, size_t N>
void register_multimethod(function<SigT> const& f, const std::type_index(&tis_)[N])
{
    return register_multimethod<MethodIDT, SigT>(f, std::make_index_sequence<N>(), tis_);
}

template <typename MethodIDT, typename SigT, typename ... TS>
function<SigT> const* get_multimethod()
{
    using sig_t = multimethod_detail::multimethod_sig<MethodIDT, SigT>;
    multimethod const* mm = sonia::services::get_multimethod({ sig_t::ti(), typeid(TS) ... });
    return mm ? &static_cast<concrete_multimethod<SigT> const&>(*mm)() : nullptr;
}

template <typename MethodIDT, typename SigT, size_t ... I, class ArrT>
function<SigT> const* get_multimethod(std::index_sequence<I ...>, ArrT const& arr)
{
    using sig_t = multimethod_detail::multimethod_sig<MethodIDT, SigT>;
    multimethod const* mm = sonia::services::get_multimethod({ sig_t::ti(), arr[I] ... });
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

//template <typename MethodIDT, typename SigT>
//function<SigT> const* get_multimethod(std::initializer_list<std::type_index> l)
//{
//    static const size_t ilsz = l.size();
//    return get_multimethod2<MethodIDT, SigT>(std::make_index_sequence<ilsz>(), {typeid(int)});
//    /*
//    using sig_t = multimethod_detail::multimethod_sig<MethodIDT, SigT>;
//
//    std::make_index_sequence<N>()
//
//    multimethod const* mm = sonia::services::get_multimethod({sig_t::ti(), tis ...});
//
//    return mm ? &static_cast<concrete_multimethod<SigT> const&>(*mm)() : nullptr;
//    */
//}

//template<TupleT>
//multimethod_apply()

}
