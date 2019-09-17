//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>

#include <vector>

#include "sonia/utility/serialization/placeholder.hpp"
#include "sonia/utility/serialization/reference.hpp"
#include "sonia/utility/serialization/integral.hpp"
#include "sonia/utility/serialization/string.hpp"

#include "sonia/utility/marshaling/marshaling.hpp"
#include "sonia/utility/marshaling/string.hpp"

using namespace sonia;

namespace {

std::string test_function0(int iv, std::string const& v)
{
    return std::to_string(iv) + "_" + v;
}

}

namespace sonia {

template <typename SigT, SigT FuncV, size_t ... PlaceHolderIndexVs>
struct binding_tag_facade2
{
    using f_type = typename boost::function_types::function_type<SigT>::type;
    using result_type = typename boost::function_types::result_type<f_type>::type;
    using args_type = function_parameter_types_t<f_type>;

    using result_transformer_t = result_transformer<args_type>;

    typedef result_type(stub_invoker_type)(mpl::at_c_t<args_type, PlaceHolderIndexVs> ...);

    using placeholder_indexes_t = mpl::vector_c<int, PlaceHolderIndexVs ...>;
    using enumerated_args_t = mpl::zip_view<mpl::vector<args_type, mpl::range_c<int, 0, mpl::size_v<args_type>>>>;

    //using placeholder_indexes_auxmin_t = mpl::push_back_t< placeholder_indexes_t, integral_constant<int, 0>>;
    //static_assert(mpl::deref_t<mpl::min_element_t<placeholder_indexes_auxmin_t>>::value >= 0);
    using placeholder_indexes_auxmax_t = mpl::push_back_t<placeholder_indexes_t, integral_constant<int, -1>>;

    static_assert(mpl::deref_t<mpl::max_element_t<placeholder_indexes_auxmax_t>>::value < int(mpl::size_v<args_type>));

    using proxy_arg_types_t = mpl::fold_t<
        enumerated_args_t,
        mpl::vector<>,
        switchable<
            mpl::contains<placeholder_indexes_t, mpl::at<mpl::_2, integral_constant<int, 1>>>,
            mpl::_1,
            mpl::push_back<mpl::_1, mpl::at<mpl::_2, integral_constant<int, 0>>>
        >
    >;
};

}

#define SONIA_DECLARE_BINDING_TAG2(tagnm, func, ...) \
struct tagnm : ::sonia::binding_tag_facade2<decltype(func), func, ## __VA_ARGS__> {};

BOOST_AUTO_TEST_CASE(marshaling_test)
{
    SONIA_DECLARE_BINDING_TAG(tag0_type, &test_function0);
    //std::cout << typeid(tag0_type::enumerated_args_t).name() << "\n\n";
    //std::cout << typeid(tag0_type::proxy_arg_types_t).name() << "\n\n";
    //std::cout << tag0_type::v0 << ", " << tag0_type::v1 << "\n";

    SONIA_DECLARE_BINDING_TAG(tag1_type, &test_function0, 0);
    //SONIA_DECLARE_BINDING_TAG(tag2_type, &test_function0, 1, 0);

    std::vector<char> data;
    tag0_type::proxy_encode<serialization::compressed_t>(std::back_inserter(data), 0, std::string("0"));
    std::string res = tag0_type::stub_invoke<serialization::compressed_t>(data.begin());
    BOOST_CHECK_EQUAL(res, "0_0");

    data.clear();
    tag1_type::proxy_encode<serialization::compressed_t>(std::back_inserter(data), std::string("1"));
    std::string res2 = tag1_type::stub_invoke<serialization::compressed_t>(data.begin(), 1);
    //BOOST_CHECK_EQUAL(res2, "1_1");

    //data.clear();
    //tag2_type::proxy_encode<serialization::compressed_t>(std::back_inserter(data));
    //std::string res3 = tag2_type::stub_invoke<serialization::compressed_t>(data.begin(), std::string("2"), 2);
    //BOOST_CHECK_EQUAL(res3, "2_2");
}
