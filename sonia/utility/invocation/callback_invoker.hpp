//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/invocation/invocation.hpp"

namespace sonia::invocation {

class callback_invoker
{
public:
    virtual ~callback_invoker() = default;

    virtual smart_blob invoke(blob_result& ftor, span<const blob_result> args) noexcept = 0;
    
    inline smart_blob invoke(blob_result& ftor, std::initializer_list<const blob_result> args) noexcept
    {
        return invoke(ftor, span<const blob_result>{ args });
    }
};

}
