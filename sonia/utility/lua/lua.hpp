//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include "sonia/span.hpp"
#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/function.hpp"

#include "sonia/utility/invokation/invokation.hpp"

namespace sonia::lua {

class language
{
public:
    class resolver
    {
    public:
        virtual blob_result index(string_view key) = 0;
        virtual bool newindex(string_view key, blob_result && value) = 0;
        virtual blob_result invoke(string_view name, span<const blob_result> args) = 0;
    };

    language();
    virtual ~language();

    void append_code(std::string code);
    cstring_view append_inplace(string_view code, bool no_return); // returns function name
    [[nodiscard]] blob_result eval_inplace(cstring_view name, std::span<const blob_result> args = {}, resolver* r = nullptr);
    [[nodiscard]] blob_result eval(string_view code, resolver* r = nullptr);
    
    [[nodiscard]] blob_result get_global_property(cstring_view name) const;
    void set_global_property(cstring_view, blob_result const& val);

public:
    int resolve_global();
    int set_global();
    int invoke_global(string_view fname, std::span<blob_result> args);

private:
    void* L_;
    std::vector<resolver*> resolvers_;
    boost::unordered_map<std::string, std::string, hasher, string_equal_to> inplace_fns_;
    boost::unordered_set<std::string, hasher, string_equal_to> codes_;
};

}
