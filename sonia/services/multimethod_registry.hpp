//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_MULTIMETHOD_REGISTRY_HPP
#define SONIA_SERVICES_MULTIMETHOD_REGISTRY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <vector>
#include <typeindex>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include "sonia/utility/concurrency/rw_fiber_mutex.hpp"
#include "sonia/utility/functional/hash/vector.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"
//#include "sonia/utility/object_pool.hpp"

#include "sonia/utility/multimethod.hpp"

namespace sonia { namespace services {

class multimethod_registry
{
    using mmholder_t = automatic_polymorphic<multimethod, 2 * sizeof(void*) + sizeof(function<void()>)>;

    struct mm_item
    {
        mmholder_t mm;
        std::vector<std::type_index> id;
    };

    using mm_set_t = boost::multi_index::multi_index_container<
        mm_item,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::member<mm_item, std::vector<std::type_index>, &mm_item::id>,
                sonia::hash<std::vector<std::type_index>>
            >
        >
    >;

public:
    void register_multimethod(multimethod &&, array_view<const std::type_index>);
    multimethod const* get_multimethod(array_view<const std::type_index>) const;

private:
    mutable sonia::fibers::rw_mutex mm_item_mtx_;
    mm_set_t mm_set_;

    //struct mm_item_id_equal {
    //    bool operator()(type_info const& id0, type_info const& id1) const { return id0 == id1; }
    //    //bool operator()(mm_item const& item0, mm_item const& item1) const { return item0.id == item1.id; }
    //    //bool operator()(mm_item const& item, type_info const& id) const { return item.id == id; }
    //    //bool operator()(type_info const& id, mm_item const& item) const { return item.id == id; }
    //};

    //object_pool<mm_item> mm_item_pool_;
    //boost::unordered_map < ti_array_t, mmholder_t, range_hasher > mm_;
};

}}

#endif // SONIA_SERVICES_MULTIMETHOD_REGISTRY_HPP
