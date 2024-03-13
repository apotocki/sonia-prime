//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "builder_view_model.hpp"

#include "sonia/beng/parser.hpp"

#include "sonia/beng/ast/declaration_visitor.hpp"

#include <boost/container/small_vector.hpp>
#include "boost/conversion/unicode/utf.hpp"
#include "boost/conversion/push_iterator.hpp"

//#include "sonia/beng/entities/variable_entity.hpp"

namespace sonia {






/*
class model_builder_context : public vm::builder_context {
    using base_t = vm::builder_context;
    builder_view_model & b_;

public:
    explicit model_builder_context(builder_view_model & b)
        : base_t{ b.vm() }, b_{b}
    {}

    void set_property(string_view id, string_view propname, blob_result const& value) override
    {
        b_.set_property(id, propname, value)
    }
};
*/



}
