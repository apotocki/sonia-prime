//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <typeindex>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include "sonia/span.hpp"
#include "sonia/small_vector.hpp"

#include "sonia/utility/concurrency/rw_fiber_mutex.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"

#include "sonia/utility/multimethod.hpp"

namespace sonia::services {

class multimethod_registry
{
public:
    using mmholder_t = automatic_polymorphic<multimethod, 2 * sizeof(void*) + sizeof(function<void()>)>;

    using mm_id_stored_elem_t = variant<std::type_index, small_string>;

    using key_set_t = boost::container::small_vector<mm_id_stored_elem_t, 4>;
    struct mm_item
    {
        mmholder_t mm;
        key_set_t id;
    };

    using mm_set_t = boost::multi_index::multi_index_container<
        mm_item,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::member<mm_item, key_set_t, &mm_item::id>,
                sonia::hash<key_set_t>
            >,
            boost::multi_index::ordered_unique<
                boost::multi_index::member<mm_item, key_set_t, &mm_item::id>,
                range_less
            >
        >
    >;

public:
    void register_multimethod(multimethod &&, span<const mm_id_elem_t>);
    multimethod const* get_multimethod(span<const mm_id_elem_t>) const;

    void copy_multimethods(span<const mm_id_elem_t> from, span<const mm_id_elem_t> to);

private:
    mutable sonia::fibers::rw_mutex mm_item_mtx_;
    mm_set_t mm_set_;
};

}
