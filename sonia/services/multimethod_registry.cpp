//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/utility/streaming/array_view.hpp"
#include "sonia/utility/streaming/type_index.hpp"

#include "multimethod_registry.hpp"

namespace sonia { namespace services {

void multimethod_registry::register_multimethod(multimethod && mm, array_view<const std::type_index> mmid)
{
    std::vector<std::type_index> vid(mmid.begin(), mmid.end());
    lock_guard slguard(mm_item_mtx_);
    auto it = mm_set_.find(vid);
    if (it == mm_set_.end()) {
        mm_set_.insert(it, {mmholder_t(std::move(mm)), std::move(vid)});
    } else {
        throw internal_error("multimethod with signature %1% is already registered"_fmt % mmid);
    }
}

multimethod const* multimethod_registry::get_multimethod(array_view<const std::type_index> mmid) const
{
    shared_lock_guard slguard(mm_item_mtx_);
    auto it = mm_set_.find(mmid, hasher(), range_equal());
    return (it != mm_set_.cend()) ? it->mm.get_pointer() : nullptr;
}

}}