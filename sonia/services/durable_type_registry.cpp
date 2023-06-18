//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/concurrency.hpp"

#include "sonia/utility/type_durable_id.hpp"
#include "durable_type_registry.hpp"

namespace sonia::services {

uint32_t durable_type_registry::register_durable_id(std::string_view nm, std::string_view servnm, std::type_index ti)
{
    uint32_t result = type_registry_->get_type_id(nm, servnm);
    lock_guard guard(type_durable_id_mtx_);
    auto rpair = type_durable_id_map_.insert(type_id_map_type::value_type(ti, result));
    if (rpair.second || rpair.first->right == result) return result;
    throw internal_error("type registration error, type %1% with name %2% has id %3% instead of %4%"_fmt % ti.name() % nm % rpair.first->right % result);
}

uint32_t durable_type_registry::get_durable_id(std::type_index ti)
{
    shared_lock_guard guard(type_durable_id_mtx_);
    auto it = type_durable_id_map_.left.find(ti);
    if (BOOST_LIKELY(it != type_durable_id_map_.left.end())) {
        return it->second;
    }
    throw internal_error("durable type %1% is not registered"_fmt % ti.name());
}

optional<std::type_index> durable_type_registry::try_get_durable_type_index(uint32_t id)
{
    shared_lock_guard guard(type_durable_id_mtx_);
    auto it = type_durable_id_map_.right.find(id);
    if (BOOST_LIKELY(it != type_durable_id_map_.right.end())) {
        return it->second;
    }
    return nullopt;
}

std::type_index durable_type_registry::get_durable_type_index(uint32_t id)
{
    auto res = try_get_durable_type_index(id);
    if (BOOST_LIKELY(!!res)) {
        return *res;
    }

    auto [name, meta] = type_registry_->get_type_description(id);
    load_durable_id(name, meta);
    res = try_get_durable_type_index(id);
    if (BOOST_LIKELY(!!res)) {
        return *res;
    }

    THROW_INTERNAL_ERROR("durable type %1% is not registered"_fmt % id);
}

}