//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/utility/streaming/type_index.hpp"

#include "multimethod_registry.hpp"
#include "sonia/logger/logger.hpp"

namespace sonia {

template <>
struct less_unrelated<std::type_index, small_string>
{
    static constexpr bool value = true;
};

template <>
struct less_unrelated<std::type_index, string_view>
{
    static constexpr bool value = true;
};

template <>
struct less_unrelated<small_string, std::type_index>
{
    static constexpr bool value = false;
};

template <>
struct less_unrelated<string_view, std::type_index>
{
    static constexpr bool value = false;
};

}

namespace sonia::services {


void multimethod_registry::register_multimethod(multimethod && mm, span<const mm_id_elem_t> mmid)
{
    //GLOBAL_LOG_INFO() << "SET " << mmid;
    key_set_t vid(mmid.begin(), mmid.end());
    lock_guard slguard(mm_item_mtx_);
    auto it = mm_set_.find(vid);
    if (it == mm_set_.end()) {
        auto iit = mm_set_.insert(it, { mmholder_t(std::move(mm)), std::move(vid) });
        BOOST_ASSERT(range_equal()(iit->id, mmid));
    } else {
        throw internal_error("multimethod with signature %1% is already registered"_fmt % mmid);
    }
}

multimethod const* multimethod_registry::get_multimethod(span<const mm_id_elem_t> mmid) const
{
    //GLOBAL_LOG_INFO() << "GET " << mmid;
    shared_lock_guard slguard(mm_item_mtx_);
    auto it = mm_set_.find(mmid, hasher(), range_equal());
    return (it != mm_set_.cend()) ? it->mm.get_pointer() : nullptr;

    return nullptr;
}

void multimethod_registry::copy_multimethods(span<const mm_id_elem_t> from, span<const mm_id_elem_t> to)
{
    std::vector<std::pair<key_set_t, mmholder_t>> copied_methods;
    shared_lock_guard slguard(mm_item_mtx_);
    auto it = mm_set_.get<1>().lower_bound(from, sonia::range_less());
    auto eit = mm_set_.get<1>().end();
    for (; it != eit && it->id.size() >= from.size(); ++it) {
        auto findee_it = it->id.begin();
        if (![from, &findee_it]() -> bool {
            auto from_it = from.begin(), from_eit = from.end();
            for (; from_it != from_eit; ++findee_it, ++from_it) {
                if (!equal_f(*findee_it, *from_it)) return false;
            }
            return true;
        }()) break;
        
        // matched
        copied_methods.emplace_back(key_set_t{to.begin(), to.end()}, it->mm);
        // supplement key
        key_set_t& k = copied_methods.back().first;
        k.insert(k.end(), findee_it, it->id.end());
    }
    for (auto & pair : copied_methods) {
        auto it = mm_set_.find(pair.first, hasher(), range_equal());
        if (it == mm_set_.end()) {
            mm_set_.insert(it, { mmholder_t(std::move(pair.second)), std::move(pair.first) });
        }
        // else skip if already exists
    }
}

}
