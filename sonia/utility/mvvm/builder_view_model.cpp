//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "builder_view_model.hpp"

#include "beng/parser.hpp"

#include "beng/declaration_visitor.hpp"
#include "beng/compiler.hpp"

#include <boost/container/small_vector.hpp>
#include "boost/conversion/unicode/utf.hpp"
#include "boost/conversion/push_iterator.hpp"

#include "beng/variable_entity.hpp"

namespace sonia {

class vm_compiler_push_value_visitor : public static_visitor<size_t>
{
public:
    vm::builder_virtual_stack_machine& bvm_;

    explicit vm_compiler_push_value_visitor(vm::builder_virtual_stack_machine& bvm) : bvm_{ bvm } {}

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

class vm_compiler_visitor : public static_visitor<void>
{
public:
    lang::beng::unit & unit_;
    vm::builder_virtual_stack_machine& bvm_;

    vm_compiler_visitor(lang::beng::unit& u, vm::builder_virtual_stack_machine& bvm) 
        : unit_{ u }
        , bvm_{ bvm }
    {}

    void operator()(std::vector<lang::beng::semantic_expression_type> const& evec) const
    {
        for (auto const& e : evec) {
            apply_visitor(*this, e);
        }
    }

    void operator()(lang::beng::semantic::push_value const& pv) const
    {
        vm_compiler_push_value_visitor vis{ bvm_ };
        size_t stack_pos = apply_visitor(vis, pv.value);
        bvm_.append_push(stack_pos);
    }

    void operator()(lang::beng::semantic::invoke_function const& invf) const
    {
        if (lang::beng::type_entity const* pte = dynamic_cast<lang::beng::type_entity const*>(invf.entity); pte) {
            bvm_.append_object_constructor();
            return;
        }
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    void operator()(lang::beng::semantic::set_variable const& pv) const
    {
        if (auto const* lve = dynamic_cast<lang::beng::local_variable_entity const*>(pv.entity); lve) {
            bvm_.append_fset(lve->index());
        } else { // extrenal variable case
            string_view varname = unit_.as_string(pv.entity->name().back());
            smart_blob strbr{ string_blob_result(varname) };
            strbr.allocate();
            size_t stack_pos = bvm_.push_on_stack(std::move(strbr));
            bvm_.append_push(stack_pos);
            bvm_.append_extern_assign();
        }
    }

    void operator()(lang::beng::semantic::push_variable const& pv) const
    {
        if (auto const* lve = dynamic_cast<lang::beng::local_variable_entity const*>(pv.entity); lve) {
            bvm_.append_fpush(lve->index());
            return;
        }
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    inline void operator()(empty_t const&) const
    { /* noop, do nothing */}

    template <typename T>
    void operator()(T const& e) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

void builder_view_model::build(fs::path const& f)
{
    lang::beng::parser_context parser{ unit_ };
    parser.parse(f);
    compile(parser);
}

void builder_view_model::build(string_view code)
{
    lang::beng::parser_context parser{ unit_ };
    parser.parse(code);
    compile(parser);
}

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

void builder_view_model::compile(lang::beng::parser_context const& pctx)
{
    auto ds = pctx.declarations();
    lang::beng::compiler_context ctx{ unit_ };
    lang::beng::declaration_visitor dvis{ ctx };
    for (auto const& d : ds) {
        apply_visitor(dvis, d);
    }
    ctx.finish_frame();
    // expression tree to vm script
    vm_compiler_visitor vmcvis{ unit(), bvm_ };

    for (auto const& e : ctx.expressions) {
        apply_visitor(vmcvis, e);
    }

    bvm_.run(*this, 0);
    // ...
    // execute script
    // ...
}

}
