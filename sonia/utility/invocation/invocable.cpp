//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "invocable.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia::invocation {

bool invocable::has_method(string_view methodname) const
{
    return !!sonia::services::get_multimethod({ get_type_index(), methodname });
}

smart_blob invocable::invoke(string_view methodname, span<const blob_result> args) noexcept
{
    smart_blob result;
    if (!try_invoke(methodname, args, result)) {
        auto errstr = ("invocable error: method '%1%' of %2% is not registered"_fmt % methodname % get_type_index().name()).str();
        result = error_blob_result(errstr);
        result.allocate();
    }
    return result;
}

bool invocable::try_invoke(string_view methodname, span<const blob_result> args, smart_blob& result) noexcept
{
    method const* pm = static_cast<method const*>(sonia::services::get_multimethod({ get_type_index(), methodname }));
    if (!pm) return false;
    result = (*pm)(*this, args); // noexcept
    return true;
}

smart_blob invocable::get_property(string_view propname) const
{
    smart_blob result;
    if (!try_get_property(propname, result)) {
        throw exception("property '%1%' of %2% is not registered"_fmt % propname % get_type_index().name());
    }
    return result;
}

bool invocable::try_get_property(string_view propname, smart_blob & result) const
{
    fn_property_reader const* pp = static_cast<fn_property_reader const*>(sonia::services::get_multimethod({ get_type_index(), typeid(fn_property_reader), propname }));
    if (!pp) return false;
    result = pp->get(*this);
    return true;
}

void invocable::set_property(string_view propname, blob_result const& val)
{
    if (!try_set_property(propname, val)) {
        throw exception("property '%1%' of %2% is not registered"_fmt % propname % get_type_index().name());
    }
}

bool invocable::try_set_property(string_view propname, blob_result const& val)
{
    fn_property_writer const* pp = static_cast<fn_property_writer const*>(sonia::services::get_multimethod({ get_type_index(), typeid(fn_property_writer), propname }));
    if (!pp) return false;
    pp->set(*this, propname, val);
    return true;
}

void invocable::set_property(string_view propname, blob_result&& val)
{
    SCOPE_EXIT([&val]() {  blob_result_unpin(&val); });
    set_property(propname, std::as_const(val));
}

}
