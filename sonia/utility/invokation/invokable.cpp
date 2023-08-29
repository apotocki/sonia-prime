//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "invokable.hpp"

namespace sonia::invokation {

bool invokable::has_method(string_view methodname)
{
    return !!sonia::services::get_multimethod({ typeid(*this), methodname });
}

smart_blob invokable::invoke(string_view methodname, span<const blob_result> args)
{
    method const* pm = static_cast<method const*>(sonia::services::get_multimethod({ typeid(*this), methodname }));
    if (!pm) {
        THROW_INTERNAL_ERROR("method %1% of %2% is not registered"_fmt % methodname % typeid(*this).name());
    }
    return (*pm)(*this, args);
}

smart_blob invokable::get_property(string_view propname) const
{
    fn_property const* pp = static_cast<fn_property const*>(sonia::services::get_multimethod({ typeid(*this), propname }));
    if (!pp) THROW_INTERNAL_ERROR("property %1% of %2% is not registered"_fmt % propname % typeid(*this).name());
    return pp->get(*this);
}

void invokable::set_property(string_view propname, blob_result const& val)
{
    fn_property const* pp = static_cast<fn_property const*>(sonia::services::get_multimethod({ typeid(*this), propname }));
    if (!pp) THROW_INTERNAL_ERROR("property %1% of %2% is not registered"_fmt % propname % typeid(*this).name());
    bool updated = pp->set(*this, val);
    if (updated) {
        on_propety_change(propname);
    }
}

}
