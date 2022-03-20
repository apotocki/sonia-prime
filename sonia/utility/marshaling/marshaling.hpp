//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/zip_view.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/max_element.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/begin.hpp>

#include <boost/mpl/fold.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/push_front.hpp>

#include <boost/bind/placeholders.hpp>

#include "sonia/optional.hpp"

#include "sonia/mpl/vector.hpp"
#include "sonia/mpl/transform_view.hpp"
#include "sonia/mpl/boost_mpl.hpp"

#include "sonia/utility/bind.hpp"
#include "sonia/utility/automatic.hpp"
#include "sonia/utility/type_durable_id.hpp"
#include "sonia/utility/serialization/tuple.hpp"
#include "sonia/utility/serialization/mpl_sequence.hpp"
#include "sonia/utility/serialization/reference.hpp"

#include "sonia/mpl/sequence.hpp"
#include "stub_parameter.hpp"

namespace sonia {

template <class SeqT, template <class...> class PredicateT, class IndexSeqT = std::make_index_sequence<boost::mpl::size<SeqT>::value>>
struct compose_tuple;

template <class SeqT, template <class...> class PredicateT, size_t ... I>
struct compose_tuple<SeqT, PredicateT, std::index_sequence<I...>>
{
    using type = std::tuple<typename PredicateT<typename boost::mpl::at_c<SeqT, I>::type>::type ...>;
};

template <class SeqT, template <class...> class PredicateT>
using composed_tuple_t = typename compose_tuple<SeqT, PredicateT>::type;

// stub routine
//template <typename SigT> struct stub_tuple_composer;
//
//template <typename R, typename ... ArgsT>
//struct stub_tuple_composer<R(ArgsT ...)>
//{
//    using type = std::tuple<stub_bound_parameter_t<ArgsT> ...>;
//};
//template <typename ... ArgsT> using stub_tuple_t = std::tuple<stub_bound_parameter_t<ArgsT&&> ...>;

template <class RealArgTypesT>
struct result_transformer
{
    template <typename T, size_t I>
    struct apply
    {
        using arg_type = sonia::mpl::at_t<RealArgTypesT, integral_constant<size_t, I>>;
        constexpr static bool is_fwd_v = stub_bound_parameter<arg_type>::is_modifiable;
        using type = conditional_t<is_fwd_v, T, null_t const&>;

        template <typename VT>
        auto const& operator()(VT && val) const
        {
            if constexpr (is_fwd_v) { return val; } else { return null; }
        }
        
        //template <typename ReferenceT>
        //auto const& operator()(stub_reference_holder<ReferenceT> const& val) const { return val.value; }

        //template <typename SomeT>
        //null_t const& operator()(SomeT const&) const { return null; }

    };
};

//using
//using proxy_decode_transformer = stub_encode_transformer;

template <typename SigT, SigT FuncV, size_t ... PlaceHolderIndexVs>
struct binding_tag_facade
{
    using f_type = typename boost::function_types::function_type<SigT>::type;
    using result_type = typename boost::function_types::result_type<f_type>::type;
    using args_type = typename boost::function_types::parameter_types<f_type>::type;

    using result_transformer_t = result_transformer<args_type>;

    typedef result_type(stub_invoker_type)(typename boost::mpl::at_c<args_type, PlaceHolderIndexVs>::type ...);

    using placeholder_indexes_t = boost::mpl::vector_c<int, PlaceHolderIndexVs ...>;
    using enumerated_args_t = boost::mpl::zip_view<boost::mpl::vector2<args_type, boost::mpl::range_c<int, 0, boost::mpl::size<args_type>::value>>>;

    //using placeholder_indexes_auxmin_t = typename boost::mpl::push_back< placeholder_indexes_t, boost::mpl::int_<0>>::type;
    //static_assert(boost::mpl::deref<typename boost::mpl::min_element<placeholder_indexes_auxmin_t>::type>::type::value >= 0);
    using placeholder_indexes_auxmax_t = typename boost::mpl::push_back<placeholder_indexes_t, boost::mpl::int_<-1>>::type;
    static_assert(boost::mpl::deref<typename boost::mpl::max_element<placeholder_indexes_auxmax_t>::type>::type::value < boost::mpl::size<args_type>::value);

    using proxy_arg_types_t = typename boost::mpl::fold<
        enumerated_args_t,
        boost::mpl::vector0<>,
        boost::mpl::if_<
            boost::mpl::contains<placeholder_indexes_t, boost::mpl::at<boost::mpl::_2, boost::mpl::int_<1>>>,
            boost::mpl::_1,
            boost::mpl::push_back<boost::mpl::_1, boost::mpl::at<boost::mpl::_2, boost::mpl::int_<0>>>
        >
    >::type;

    using proxy_invoker_sig_seq_type = typename boost::mpl::push_front<proxy_arg_types_t, result_type>::type;
    using proxy_invoker_type = typename boost::function_types::function_type<proxy_invoker_sig_seq_type>::type;

    using stub_arg_types_t = typename boost::mpl::fold<
        enumerated_args_t,
        boost::mpl::vector0<>,
        boost::mpl::push_back<boost::mpl::_1,
            boost::mpl::if_<
                boost::mpl::contains<placeholder_indexes_t, boost::mpl::at<boost::mpl::_2, boost::mpl::int_<1>>>,
                arg<boost::mpl::plus<boost::mpl::int_<1>, boost::mpl::distance<
                    typename boost::mpl::begin<placeholder_indexes_t>::type,
                    boost::mpl::find<placeholder_indexes_t, boost::mpl::at<boost::mpl::_2, boost::mpl::int_<1>>>
                >>>,
                boost::mpl::at<boost::mpl::_2, boost::mpl::int_<0>>
            >
        >
    >::type;

    using stub_tuple_t = composed_tuple_t<stub_arg_types_t, stub_bound_parameter>;

    template <typename EncT, typename OutputIteratorT, typename ... ArgsT>
    static OutputIteratorT proxy_encode(OutputIteratorT oi, ArgsT && ... args)
    {
        static_assert(is_same_v<result_type, decltype(std::invoke(std::declval<proxy_invoker_type>(), std::forward<ArgsT>(args)...))>);
        return encode<EncT>(make_bind_tuple(std::forward<ArgsT>(args)...), std::move(oi));
    }

    template <typename CoderTagT, typename InputIteratorT, typename ... ArgsT>
    static result_type stub_invoke(InputIteratorT ii, ArgsT && ... args)
    {
        using r_type = decltype(std::apply(std::declval<stub_invoker_type>(), bind_tuple_t<ArgsT...>(std::declval<ArgsT>()...)));
        static_assert(is_same_v<result_type, r_type>);

        automatic<stub_tuple_t> stub_tuple{in_place_decode<CoderTagT>(ii)};

        return apply_placeholders(FuncV, *stub_tuple, std::forward<ArgsT>(args)...);
    }

    template <typename CoderTagT, typename InputIteratorT, typename OutputIteratorT, typename ... ArgsT>
    static void stub_invoke_and_encode(InputIteratorT & ii, OutputIteratorT & oi, ArgsT && ... args)
    {
        using r_type = decltype(std::apply(std::declval<stub_invoker_type>(), bind_tuple_t<ArgsT...>(std::declval<ArgsT>()...)));
        static_assert(is_same_v<result_type, r_type>);

        automatic<stub_tuple_t> stub_tuple{in_place_decode<CoderTagT>(ii)};
        
        auto error_handler = [&oi](std::exception const& e) {
            string_view err{e.what()};
            oi = (make_encoder<CoderTagT>(std::move(oi)) & err.size() & err).iterator();
        };

        if constexpr (!is_void_v<result_type>)
        {
            optional<conditional_t<is_void_v<result_type>, void*, result_type>> r;
            try { r = apply_placeholders(FuncV, *stub_tuple, std::forward<ArgsT>(args)...); } catch (std::exception const& e) { error_handler(e); return; }
            oi = sonia::encode<CoderTagT, size_t>(0, std::move(oi));
            oi = sonia::encode<CoderTagT>(*r, std::move(oi));
        } else {
            try { apply_placeholders(FuncV, *stub_tuple, std::forward<ArgsT>(args)...); } catch (std::exception const& e) { error_handler(e); return; }
            oi = sonia::encode<CoderTagT, size_t>(0, std::move(oi));
        }
        oi = sonia::encode<CoderTagT>(sonia::mpl::make_transform_view<result_transformer_t>(*stub_tuple), std::move(oi));
    }
};

}

#define SONIA_DECLARE_BINDING_TAG(tagnm, func, ...) \
struct tagnm : ::sonia::binding_tag_facade<decltype(func), func, ## __VA_ARGS__> {};

#define SONIA_DECLARE_BINDING_PTAG(tags, tagnm, func, ...) \
template <class ... tags> struct tagnm : ::sonia::binding_tag_facade<decltype(func), func, ## __VA_ARGS__> {};

#define SONIA_REGISTER_BINDING_TAG(tag, tagnm, serv) \
::sonia::services::register_durable_id(tagnm, serv, typeid(tag))
