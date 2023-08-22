//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "registration.hpp"

namespace sonia::invokation {

struct comparer
{
    template <typename LeftStringT, typename RightStringT>
    inline bool operator()(std::pair<std::type_index, LeftStringT> const& l, std::pair<std::type_index, RightStringT> const& r) const
    {
        return l.first == r.first && l.second == r.second;
    }
};

void registry::register_method(std::type_index ti, std::string_view name, method_holder mh)
{
    {
        lock_guard guard(mtx_);
        auto it = concrete_methods_map_.find(std::pair{ti, name}, hasher{}, comparer{});
        if (it != concrete_methods_map_.end()) {
            THROW_INTERNAL_ERROR("method %1% of %2% is already registered"_fmt % name % ti.name());
        }
        concrete_methods_map_.insert(it, { std::pair{ti, std::string{name}}, mh });
    }

    lock_guard guard(methods_map_mtx_);
    auto it = methods_map_.find(ti);
    if (it != methods_map_.end()) {
        it->second.emplace_back(std::string{name}, std::move(mh));
    } else {
        methods_map_.insert(it, methods_map_t::value_type{ ti, { std::pair{std::string{name}, std::move(mh)} } });
    }
}

void registry::register_property(std::type_index ti, std::string_view name, fn_property_holder mh)
{
    {
        lock_guard guard(props_mtx_);
        auto it = concrete_properties_.find(std::pair{ ti, name }, hasher{}, comparer{});
        if (it != concrete_properties_.end()) {
            THROW_INTERNAL_ERROR("property %1% of %2% is already registered"_fmt % name % ti.name());
        }
        concrete_properties_.insert(it, { std::pair{ti, std::string{name}}, mh });
    }
}

void registry::inherit(std::type_index from, std::type_index to)
{
    std::vector<std::pair<std::string, method_holder>>* ph = nullptr;
    {
        lock_guard guard(methods_map_mtx_);
        auto it = methods_map_.find(from);
        if (it == methods_map_.end()) {
            THROW_INTERNAL_ERROR("no method is registered for %1%"_fmt % from.name());
        }
        ph = &it->second;
    }
    for (auto & pair : *ph) {
        register_method(to, pair.first, pair.second);
    }
}

blob_result registry::invoke(std::type_index ti, invokable& obj, std::string_view name, std::span<const blob_result> args)
{
    method* pm = nullptr;
    {
        lock_guard guard(mtx_);
        auto it = concrete_methods_map_.find(std::pair{ti, name}, hasher{}, comparer{});
        if (it == concrete_methods_map_.end()) {
            THROW_INTERNAL_ERROR("method %1% of %2% is not registered"_fmt % name % ti.name());
        }
        pm = it->second.get_pointer();
    }
    return (*pm)(obj, args);
}

bool registry::has_method(std::type_index ti, std::string_view methodname)
{
    lock_guard guard(mtx_);
    return concrete_methods_map_.end() != concrete_methods_map_.find(std::pair{ti, methodname}, hasher{}, comparer{});
}

blob_result registry::get_property(std::type_index ti, invokable const& obj, std::string_view propname) const
{
    fn_property const* pp = nullptr;
    {
        lock_guard guard(props_mtx_);
        auto it = concrete_properties_.find(std::pair{ ti, propname }, hasher{}, comparer{});
        if (it == concrete_properties_.end()) {
            THROW_INTERNAL_ERROR("get_property: error: property %1% of %2% is not registered"_fmt % propname % ti.name());
        }
        pp = it->second.get_pointer();
    }
    return pp->get(obj);
}

bool registry::set_property(std::type_index ti, invokable& obj, std::string_view propname, blob_result&& val)
{
    fn_property * pp = nullptr;
    {
        lock_guard guard(props_mtx_);
        auto it = concrete_properties_.find(std::pair{ ti, propname }, hasher{}, comparer{});
        if (it == concrete_properties_.end()) {
            THROW_INTERNAL_ERROR("set_property: error: property %1% of %2% is not registered"_fmt % propname % ti.name());
        }
        pp = it->second.get_pointer();
    }
    return pp->set(obj, std::move(val));
}

bool invokable::has_method(std::string_view methodname)
{
    return as_singleton<invokation::registry>()->has_method(typeid(*this), methodname);
}

blob_result invokable::invoke(std::string_view methodname, std::span<const blob_result> args)
{
    return as_singleton<invokation::registry>()->invoke(typeid(*this), *this, methodname, args);
}

blob_result invokable::get_property(std::string_view propname, bool pin) const
{
    blob_result result = as_singleton<invokation::registry>()->get_property(typeid(*this), *this, propname);
    if (pin && result.need_unpin) {
        if (auto mng = get_blob_manager(); mng) {
            mng->pin(result);
        } else {
            // it's unsafe use a result because the buffer is already released
            throw exception("the assigned manager is obsoleted");
        }
    }
    else if (!pin && result.need_unpin) {
        result.need_unpin = 0;
    }
    return result;
}

void invokable::set_property(std::string_view propname, blob_result&& val)
{
    bool updated = as_singleton<invokation::registry>()->set_property(typeid(*this), *this, propname, std::move(val));
    if (updated) {
        on_propety_change(propname);
    }
}

}
