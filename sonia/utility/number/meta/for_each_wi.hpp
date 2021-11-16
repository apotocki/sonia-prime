//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/zip_view.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/back.hpp>

namespace sonia::mpl {

namespace detail {
    
template <typename F>
struct for_each_wi_wraper_helper
{
    inline for_each_wi_wraper_helper(F const& f) : f_(f) {}

    template <typename T>
    inline void operator ()(T) {
        typedef typename boost::mpl::front<T>::type index_t;
        typedef typename boost::mpl::back<T>::type value_t;
        f_(index_t::value, value_t());
    }
private:
    F f_;
};

}

template <typename Sequence, typename F>
inline void for_each_wi(F f)
{
    typedef boost::mpl::zip_view<
        boost::mpl::vector2<
              boost::mpl::range_c<int, 0, boost::mpl::size<Sequence>::value>
            , Sequence
        >
    > tempseq;
    boost::mpl::for_each<tempseq>(detail::for_each_wi_wraper_helper<F>(f));
}

}
