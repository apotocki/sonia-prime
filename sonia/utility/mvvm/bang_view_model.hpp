//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_map.hpp>

#include "sonia/utility/mvvm/view_model.hpp"
#include "sonia/utility/invocation/invocation.hpp"
#include "sonia/bang/bang.hpp"

namespace sonia {

// it's abstract class, still virtual: get_manager, set(manager), self_as_content_view_shared
class bang_view_model
    : public virtual view_model
    , public sonia::lang::bang::language
    , public invocation::registrar<bang_view_model, view_model>
{
    using registrar_type = invocation::registrar<bang_view_model, view_model>::registrar_type;
    friend class invocation::registrar<bang_view_model, view_model>;

public:
    explicit bang_view_model(int32_t idval = 0);

    // scripting
    void load_code(string_view code);
    void load_file(string_view path);
    smart_blob eval(string_view code, bool no_return);

protected:
    // methods routine
    static void do_registration(registrar_type& mr);

private:
    boost::unordered_map<std::string, std::string, hasher, string_equal_to> inplace_fns_;
};

}
