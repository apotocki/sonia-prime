//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/filesystem.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/span.hpp"
#include "extern.hpp"

namespace sonia::lang::bang {

namespace detail { class bang_impl; }

class language
{
public:
    language();
    virtual ~language();

    void build(fs::path const&);
    void build(string_view code);

    void run(external_environment*, span<string_view> args = {});

private:
    shared_ptr<detail::bang_impl> impl_;
    
};

}
