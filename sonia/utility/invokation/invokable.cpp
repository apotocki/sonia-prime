//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "invokable.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia::invokation {

bool invokable::has_method(string_view methodname) const
{
    return !!sonia::services::get_multimethod({ typeid(*this), methodname });
}

smart_blob invokable::invoke(string_view methodname, span<const blob_result> args)
{
    smart_blob result;
    if (!try_invoke(methodname, args, result)) {
        throw exception("method %1% of %2% is not registered"_fmt % methodname % typeid(*this).name());
    }
    return result;
}

bool invokable::try_invoke(string_view methodname, span<const blob_result> args, smart_blob& result)
{
    method const* pm = static_cast<method const*>(sonia::services::get_multimethod({ typeid(*this), methodname }));
    if (!pm) return false;
    result = (*pm)(*this, args);
    return true;
}

smart_blob invokable::get_property(string_view propname) const
{
    smart_blob result;
    if (!try_get_property(propname, result)) {
        throw exception("property %1% of %2% is not registered"_fmt % propname % typeid(*this).name());
    }
    return result;
}

bool invokable::try_get_property(string_view propname, smart_blob & result) const
{
    fn_property_reader const* pp = static_cast<fn_property_reader const*>(sonia::services::get_multimethod({ typeid(*this), typeid(fn_property_reader), propname }));
    if (!pp) return false;
    result = pp->get(*this);
    return true;
}

void invokable::set_property(string_view propname, blob_result const& val)
{
    if (!try_set_property(propname, val)) {
        throw exception("property %1% of %2% is not registered"_fmt % propname % typeid(*this).name());
    }
}

bool invokable::try_set_property(string_view propname, blob_result const& val)
{
    fn_property_writer const* pp = static_cast<fn_property_writer const*>(sonia::services::get_multimethod({ typeid(*this), typeid(fn_property_writer), propname }));
    if (!pp) return false;
    pp->set(*this, propname, val);
    return true;
}

void invokable::set_property(string_view propname, blob_result&& val)
{
    SCOPE_EXIT([&val]() {  blob_result_unpin(&val); });
    set_property(propname, std::as_const(val));
}

}
