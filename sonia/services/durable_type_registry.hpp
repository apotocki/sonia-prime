//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_DURABLE_TYPE_REGISTRY_HPP
#define SONIA_SERVICES_DURABLE_TYPE_REGISTRY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <typeindex>

#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>

#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/utility/concurrency/rw_fiber_mutex.hpp"

#include "type_registry.hpp"

namespace sonia { namespace services {

class durable_type_registry
{
    using type_id_map_type = boost::bimap<
        boost::bimaps::unordered_set_of<std::type_index>,
        boost::bimaps::unordered_set_of<uint32_t>
    >;

public:
    uint32_t register_durable_id(string_view nm, string_view servnm, std::type_index);
    uint32_t get_durable_id(std::type_index);
    std::type_index get_durable_type_index(uint32_t);

    optional<std::type_index> try_get_durable_type_index(uint32_t id);

    void set_type_registry(shared_ptr<type_registry> v)
    {
        type_registry_ = std::move(v);
    }

private:
    shared_ptr<type_registry> type_registry_;
    sonia::fibers::rw_mutex type_durable_id_mtx_;
    type_id_map_type type_durable_id_map_;
};

}}

#endif // SONIA_SERVICES_DURABLE_TYPE_REGISTRY_HPP
