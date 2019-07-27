//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include <boost/test/unit_test.hpp>

#include <boost/mpl/assert.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/mpl/push_back.hpp"
#include "sonia/mpl/fold.hpp"

using namespace sonia;

namespace sonia::mpl {

using _1 = arg_c<1>;
using _2 = arg_c<2>;

}

BOOST_AUTO_TEST_CASE (mpl_test)
{
    using types = std::tuple<long, float, short, double, float, long, long double>;
    using number_of_floats = mpl::fold_t<
        types,
        integral_constant<int, 0>,
        conditional<is_floating_point_v<mpl::_2>, next<mpl::_1>, mpl::_1>
    >;

    BOOST_MPL_ASSERT_RELATION( number_of_floats::value, ==, 4 );
}
