//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"

#include "beng_vm.hpp"

namespace sonia::beng::vm {

class push_value_visitor : public static_visitor<size_t>
{
public:
    virtual_stack_machine& bvm_;

    explicit push_value_visitor(virtual_stack_machine& bvm) : bvm_{ bvm } {}

    inline size_t operator()(null_t const&) const
    {
        return bvm_.push_on_stack(smart_blob{});
    }

    size_t operator()(small_u32string const& sval) const
    {
        namespace cvt = boost::conversion;
        boost::container::small_vector<char, 32> result;
        result.reserve(sval.size());
        (cvt::cvt_push_iterator(cvt::utf32 | cvt::utf8, std::back_inserter(result)) << sval).flush();
        smart_blob strbr{ string_blob_result(span{result}) };
        strbr.allocate();
        return bvm_.push_on_stack(std::move(strbr));
    }

    size_t operator()(decimal const& dval) const
    {
        if (dval.raw_exp() >= 0) { // is integral
            if (dval >= (std::numeric_limits<int64_t>::min)() && dval <= (std::numeric_limits<int64_t>::max)()) {
                return bvm_.push_on_stack(smart_blob{ i64_blob_result((int64_t)dval) });
            } else if (dval >= 0 && dval <= (std::numeric_limits<uint64_t>::max)()) {
                return bvm_.push_on_stack(smart_blob{ ui64_blob_result((uint64_t)dval) });
            }
        }
        return bvm_.push_on_stack(smart_blob{ f64_blob_result((double_t)dval) });
    }

    template <typename T>
    size_t operator()(T const& e) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}
