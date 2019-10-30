//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SINGLETON_LOCATOR_HPP
#define SONIA_SINGLETON_LOCATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/unordered_map.hpp>

#include "sonia/shared_ptr.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/function.hpp"
#include "sonia/logger/loggable.hpp"
#include "sonia/services/singleton.hpp"

namespace sonia {
    
DECLARE_EXCEPTION(circular_dependency_error, "circular dependency error", internal_error)

class singleton_locator : public loggable
{
public:
    singleton_locator() { set_log_attribute("Type", "singleton_locator"); }

    explicit singleton_locator(null_t) {};

    shared_ptr<singleton> get(singleton::id id, function<shared_ptr<singleton>(singleton::id)> const& factory);

    void shutdown(shared_ptr<singleton>);
    void shutdown();

protected:
    struct singleton_descriptor
    {
        using hook_type = boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::normal_link>>;

        fibers::mutex mtx;
        fiber::id fid;
        shared_ptr<singleton> object;
        hook_type layer_hook;

        explicit singleton_descriptor(in_place_t) {}

        struct layer_compare_type
        {
            bool operator()(singleton_descriptor const& lhs, singleton_descriptor const& rhs) const noexcept
            {
                return lhs.object->get_layer() < rhs.object->get_layer();
            }
        };
    };

    using layer_set_t = boost::intrusive::multiset<
        singleton_descriptor,
        boost::intrusive::member_hook<
              singleton_descriptor
            , singleton_descriptor::hook_type
            , &singleton_descriptor::layer_hook
        >,
        boost::intrusive::compare<singleton_descriptor::layer_compare_type>
    >;

    fibers::mutex cache_mtx_, layers_mtx_;
    std::unordered_map<singleton::id, singleton_descriptor> cache_;
    layer_set_t layers_;
};

}

#endif // SONIA_SINGLETON_LOCATOR_HPP
