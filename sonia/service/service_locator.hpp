//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICE_LOCATOR_HPP
#define SONIA_SERVICE_LOCATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <unordered_map>
#include <mutex>
#include <tuple>

#include <boost/intrusive/set.hpp>

#include "sonia/thread.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/service/service.hpp"

namespace sonia {

class service_locator {
public:
    service_locator(shared_ptr<service_registry> sr, shared_ptr<service_factory> sf);
    ~service_locator() noexcept;

    shared_ptr<service> get(string_view name);
    shared_ptr<service> get(service::id id);

    void shutdown(shared_ptr<service> serv);
    // returns the shutdowned layer, can be less than an argument level
    void shutdown(int down_to_layer = std::numeric_limits<int>::min());

    struct cached_service_descriptor {
        typedef boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::normal_link> > hook_type;

        std::mutex mtx;
        thread::id tid;
        shared_ptr<service> object;
        hook_type layer_hook;

        cached_service_descriptor(std::in_place_t) {}

        int get_layer() const { return object->get_layer(); }

        struct layer_compare_type {
            bool operator()(cached_service_descriptor const& lhs, cached_service_descriptor const& rhs) const {
                return lhs.get_layer() < rhs.get_layer();
            }
        };
    };

private:
    shared_ptr<service> get(service::id id, string_view name);

    typedef boost::intrusive::multiset<
        cached_service_descriptor,
        boost::intrusive::member_hook<
              cached_service_descriptor
            , cached_service_descriptor::hook_type
            , &cached_service_descriptor::layer_hook
        >,
        boost::intrusive::compare<cached_service_descriptor::layer_compare_type>
    > layer_set_t;

    shared_ptr<service_registry> sr_;
    shared_ptr<service_factory> sf_;

    std::mutex cache_mtx_, layers_mtx_;
    std::unordered_map<service::id, cached_service_descriptor> cache_;
    layer_set_t layers_;
};

}

#endif // SONIA_SERVICE_LOCATOR_HPP
