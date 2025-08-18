//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "invocation.hpp"

namespace sonia::invocation {

class invocable
{
public:
    virtual ~invocable() = default;

    virtual std::type_index get_type_index() const { return typeid(*this); }

    virtual bool has_method(string_view methodname) const;
    virtual bool try_invoke(string_view methodname, span<const blob_result> args, smart_blob& result) noexcept;
    virtual bool try_get_property(string_view propname, smart_blob& result) const;
    virtual bool try_set_property(string_view propname, blob_result const& val);

    virtual void on_property_change(string_view) {}

    virtual shared_ptr<invocable> self_as_invocable_shared() { return {}; }

    // method routine
    inline bool try_invoke(string_view methodname, span<const blob_result> args) noexcept
    {
        smart_blob result;
        return try_invoke(methodname, args, result);
    }

    smart_blob invoke(string_view methodname, span<const blob_result> args) noexcept;
    inline smart_blob invoke(string_view name, std::initializer_list<const blob_result> args) noexcept
    {
        return invoke(name, span{args});
    }
        
    // properties routine
    smart_blob get_property(string_view propname) const;
    void set_property(string_view propname, blob_result const& val);
    void set_property(string_view propname, blob_result&& val);
};

}
