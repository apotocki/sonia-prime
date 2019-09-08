//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include <boost/test/unit_test.hpp>

#include <boost/mpl/assert.hpp>

#include "sonia/type_traits.hpp"

// sequences
#include "sonia/mpl/vector.hpp"
#include "sonia/mpl/transform_view.hpp"
#include "sonia/mpl/zip_view.hpp"

// iterators
#include "sonia/mpl/iterator.hpp"

// builders
#include "sonia/mpl/push_back.hpp"
#include "sonia/mpl/fold.hpp"

// quering
#include "sonia/mpl/max_element.hpp"
#include "sonia/mpl/contains.hpp"
#include "sonia/mpl/equal.hpp"
#include "sonia/mpl/find_if.hpp"

// metafunctions
#include "sonia/mpl/lambda.hpp"
#include "sonia/mpl/bind.hpp"
#include "sonia/mpl/apply.hpp"
#include "sonia/mpl/apply_wrap.hpp"
#include "sonia/mpl/unpack_args.hpp"
#include "sonia/mpl/less.hpp"
#include "sonia/mpl/plus.hpp"


using namespace sonia;

namespace sonia::mpl {

/////////////////////
template <typename T0, typename T1>
struct smart_equal : conditional_t<
    is_mpl_sequence_v<T0> && is_mpl_sequence_v<T1>,
    equal<T0, T1>,
    is_same<T0, T1>
>
{};

/////////////////////

/////////////////////


/////////////////////


/////////////////////

}

namespace static_compile_test_quote
{

template <typename T> struct f1 { using type = T; };

template <typename T1, typename T2, typename T3, typename T4, typename T5> struct f5 { /* no 'type' member! */ };

using t1 = sonia::mpl::quote<f1>::apply_t<int>;
using t5 = sonia::mpl::quote<f5>::apply_t<char,short,int,long,float>;

static_assert(is_same_v<t1, int>);
static_assert(is_same_v<t5, f5<char,short,int,long,float>>);

}

namespace static_compile_test_bind
{
using namespace sonia::mpl;

struct f1 { template< typename T1 > struct apply { using type = T1; }; };
struct f5 { template <typename T1, typename T2, typename T3, typename T4, typename T5> struct apply { using type = T5; }; };

using r11 = apply_wrap_t<bind<f1,_1>, int>;
static_assert(is_same_v<r11, int>);

using r12 = apply_wrap_t<bind<f1,_5>, void,void,void,void,int>;
static_assert(is_same_v<r12, int>);

using r51 = apply_wrap_t<bind<f5,_1,_2,_3,_4,_5>, void,void,void,void,int>;
static_assert(is_same_v<r51, int>);

using r52 = apply_wrap_t<bind<f5,_5,_4,_3,_2,_1>, float,void,void,void,void>;
static_assert(is_same_v<r52, float>);

}

namespace static_compile_test_protect
{
using namespace sonia::mpl;

struct f { template< typename T1, typename T2 > struct apply { using type = T2; }; };

using b1 = bind<quote<switchable>,_1,_2,bind<f,_1,_2> >;
using b2 = bind<quote<switchable>,_1,_2,protect< bind<f,_1,_2> > >;

using r1 = apply_wrap_t<b1,false_type,char>;
using r2 = apply_wrap_t<b2,false_type,char>;

static_assert(is_same_v<r1,char>);
static_assert(is_same_v<r2,protect< bind<f,_1,_2>>>);

}

#if 1
namespace static_compile_test_lambda
{
    using namespace sonia::mpl;

    template <typename N1, typename N2> struct int_plus : integral_constant<int, N1::value + N2::value> {};

    using f1 = lambda_t<int_plus<_1, integral_constant<int, 42>>>;
    using f2 = bind<quote<int_plus>, _1, integral_constant<int, 42>>;

    using r1 = f1::apply<integral_constant<int, 42>>::type;
    using r2 = f2::apply<integral_constant<int, 42>>::type;

    static_assert( r1::value == 84 );
    static_assert( r2::value == 84 );
}

#endif

#if 1
namespace static_compile_test_iter_fold
{
    using namespace sonia::mpl;

    using numbers = vector_c<int,5,-1,0,7,2,0,-5,4>;

    using max_element_iter = iter_fold_t<
          numbers
        , begin_t<numbers>
        , switchable< less< deref<_1>, deref<_2> >,_2,_1 >
    >;

    static_assert(deref_t<max_element_iter>::value == 7);
}
#endif


namespace static_compile_test_iter_fold1
{
using namespace sonia::mpl;

using vec = vector_c<int, 1, 2, 3, 4>;

using result = iter_fold_t<
    vec, 
    integral_constant<int, 0>, 
    plus<_1, deref<_2>>
>;

static_assert(result::value == 10);
}

#if 1
namespace static_compile_test_iter_fold2
{
using namespace sonia::mpl;

using vec = vector_c<int, 1, 2, 3, 4>;

using result = fold_t<
    vec, 
    integral_constant<int, 0>, 
    plus<_1, _2>
>;

static_assert(result::value == 10);

}
#endif

#if 1
namespace static_compile_test_iter_fold3
{
using namespace sonia::mpl;

using vec1 = vector_c<int, 1, 1, 1>;
using vec2 = vector_c<int, 2, 2, 2>;
using vec3 = vector_c<int, 3, 3, 3>;
using vvec = vector<vec1, vec2, vec3>;

using lam = lambda_t<fold<
    _1, 
    integral_constant<int, 0>, 
    lambda_t<plus<_1, _2>>
>>;

using result = fold_t<
    vvec, 
    integral_constant<int, 0>, 
    plus<_1, lam::apply<_2> >
>;

static_assert(result::value == 18);

}

namespace static_compile_test_max_element
{
    using namespace sonia::mpl;
    using numbers = vector_c<int,5,-1,0,9,2,0,-5,4>;

    using max_elem = deref_t<max_element_t<numbers>>;

    static_assert(max_elem::value == 9);
}

namespace static_compile_test_contains
{
    using namespace sonia::mpl;
    using numbers = vector_c<int,5,-1,0,9,2,0,-5,4>;

    using has4_t = contains_t<numbers, integral_constant<int, 4>>;
    using has5_t = contains_t<numbers, integral_constant<int, 15>>;

    static_assert(has4_t::value);
    static_assert(!has5_t::value);
}

#include "sonia/mpl/transform_view.hpp"
namespace static_compile_test_transform_view
{
    using namespace sonia::mpl;
    using types = vector<int,long,char,char[50],double>;
    using iter = max_element_t<
      transform_view< types, size_of<_1> >
    >;

    static_assert(deref_t<iter>::value == 50);
}

namespace static_compile_test_zip_view
{
    using namespace sonia::mpl;
    using v1 = vector_c<int,1,2,3,4,5>;
    using v2 = vector_c<int,5,4,3,2,1>;
    using v3 = vector_c<int,1,1,1,1,1>;

    using r = vector<
        vector_c<int, 1,5,1>,
        vector_c<int, 2,4,1>,
        vector_c<int, 3,3,1>,
        vector_c<int, 4,2,1>,
        vector_c<int, 5,1,1>
    >;

    using tv1 = transform_view<v1, identity<_1>>;
    static_assert(equal_t<v1, tv1>::value);

    using zv = zip_view<vector<v1, v2, v3>>;
    static_assert(size_v<zv> == 5);

    static_assert (equal_t<at_c_t<zv, 0>, vector_c<int,1,5,1>>::value);
    static_assert (equal_t<at_c_t<zv, 1>, vector_c<int,2,4,1>>::value);
    static_assert (equal_t<at_c_t<zv, 2>, vector_c<int,3,3,1>>::value);
    static_assert (equal_t<at_c_t<zv, 3>, vector_c<int,4,2,1>>::value);
    static_assert (equal_t<at_c_t<zv, 4>, vector_c<int,5,1,1>>::value);

    static_assert (equal_t<r, zv, smart_equal<_1, _2>>::value);

    using sum = transform_view<
          zip_view< vector<v1,v2,v3> >
        , unpack_args< plus<_1,_2,_3> >
    >;
    static_assert(size_v<sum> == 5);

    static_assert(mpl::apply_t<unpack_args<is_same<_1, _2>>, vector<int,int>>::value);
    static_assert(mpl::apply_t<unpack_args<plus<_1, _2>>, vector_c<int, 1, 2>>::value == 3);
    static_assert(mpl::apply_t<unpack_args< plus<_1,_2,_3> >, at_c_t<zv, 0>>::value==7);
    
    static_assert(at_c_t<sum, 0>::value == 7);
    static_assert(at_c_t<sum, 1>::value == 7);
    static_assert(at_c_t<sum, 2>::value == 7);
    static_assert(at_c_t<sum, 3>::value == 7);
    static_assert(at_c_t<sum, 4>::value == 7);

    static_assert(equal_t<sum, vector_c<int,7,7,7,7,7>>::value);
}

#endif

#include <iostream>
BOOST_AUTO_TEST_CASE (mpl_test)
{
    //std::cout << typeid(static_compile_test_zip_view::s0).name() << "\n";
    //std::cout << static_compile_test_iter_fold::i2::pos << "\n";
    //using types = std::tuple<long, float, short, double, float, long, long double>;
    //using number_of_floats = mpl::fold_t<
    //    types,
    //    integral_constant<int, 0>,
    //    conditional<is_floating_point_v<mpl::_2>, next<mpl::_1>, mpl::_1>
    //>;

    //BOOST_MPL_ASSERT_RELATION( number_of_floats::value, ==, 4 );
}
