//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"

#include "bang_vm.hpp"
#include "../semantic.hpp"
#include "../entities/functional_entity.hpp"

namespace sonia::lang::bang::vm {

class push_value_visitor : public static_visitor<void>, public entity_visitor
{
public:
    unit& unit_;
    auto& bvm() const { return unit_.bvm(); }

    explicit push_value_visitor(unit& u) : unit_{ u } {}

    inline void operator()(null_t const&) const
    {
        bvm().append_push_static_const(smart_blob{});
    }

    void operator()(bool bval) const
    {
        bvm().append_push_static_const(smart_blob{ bool_blob_result(bval) });
    }

    void operator()(small_string const& sval) const
    {
        //namespace cvt = boost::conversion;
        //boost::container::small_vector<char, 32> result;
        //result.reserve(sval.size());
        //(cvt::cvt_push_iterator(cvt::utf32 | cvt::utf8, std::back_inserter(result)) << sval).flush();
        smart_blob strbr{ string_blob_result(sval) };
        strbr.allocate();
        bvm().append_push_static_const(std::move(strbr));
    }

    void operator()(mp::integer const& ival) const
    {
        if (ival.is_fit<int64_t>()) {
            bvm().append_push_static_const(smart_blob{ i64_blob_result((int64_t)ival) });
        } else if (ival.is_fit<uint64_t>()) {
            bvm().append_push_static_const(smart_blob{ ui64_blob_result((uint64_t)ival) });
        } else {
            bvm().append_push_static_const(smart_blob{ bigint_blob_result(ival) });
        }
    }

    void operator()(mp::decimal const& dval) const
    {
        //THROW_NOT_IMPLEMENTED_ERROR("push_value_visitor decimal");
#if 1
        if (dval.exponent().sgn() >= 0) { // is integral
            if (dval >= mp::decimal{(std::numeric_limits<int64_t>::min)()} && dval <= mp::decimal{(std::numeric_limits<int64_t>::max)()}) {
                bvm().append_push_static_const(smart_blob{ i64_blob_result((int64_t)dval) });
            } else if (dval >= 0 /*mp::decimal{ 0}*/ && dval <= mp::decimal{ (std::numeric_limits<uint64_t>::max)()}) {
                bvm().append_push_static_const(smart_blob{ ui64_blob_result((uint64_t)dval) });
            }
        } else {
            bvm().append_push_static_const(smart_blob{ f64_blob_result((double_t)dval) });
        }
#endif
    }

    void operator()(lang::bang::function_value const& dval) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("push_value_visitor function_value");
#if 0
        if (auto eptr = unit_.eregistry().find(dval.mangled_name); eptr) {
            if (auto fe = dynamic_pointer_cast<lang::bang::function_entity>(eptr); fe) {
                if (!fe->is_defined()) {
                    size_t pos = bvm().append_static(smart_blob{}); // just reserve
                    fe->set_static_variable_index(pos);
                }
                if (fe->is_static_variable_index()) {
                    bvm().append_pushs(fe->get_address());
                    bvm().append_push_static_const(bool_blob_result(true));
                } else {
                    bvm().append_push_static_const(ui64_blob_result(fe->get_address()));
                    bvm().append_push_static_const(bool_blob_result(false));
                }
                
                if (!fe->captured_variables.empty()) {
                    for (auto const& pair : fe->captured_variables) {
                        bvm().append_fpushi(std::get<0>(pair)->index());
                        bvm().append_ecall(virtual_stack_machine::builtin_fn::referify);
                        bvm().append_fpush(std::get<0>(pair)->index());
                    }
                    if (fe->captured_variables.size() > 1) {
                        //size_t idx = bvm().push_on_stack(ui64_blob_result(fe->captured_variables.size()));
                        bvm().append_push_static_const(ui64_blob_result(fe->captured_variables.size()));
                        bvm().append_ecall(virtual_stack_machine::builtin_fn::arrayify);

                        //bvm().push_on_stack_and_push(ui64_blob_result(fe->captured_variables.size()));
                        //bvm().append_builtin(sonia::lang::bang::builtin_type::arrayify);
                    }
                } else {
                    bvm().append_push_static_const(nil_blob_result());
                }
                
                auto nmstr = unit_.print(dval.mangled_name);
                smart_blob strbr{ string_blob_result(nmstr) };
                strbr.allocate();
                bvm().append_push_static_const(std::move(strbr));
                bvm().append_push_static_const(i64_blob_result((fe->signature().parameters_count() + 1) * (fe->is_void() ? -1 : 1)));
                bvm().append_ecall(virtual_stack_machine::builtin_fn::function_constructor);
                
                //return bvm_.push_on_stack(smart_blob{ object_blob_result<function_invoker>(fe.get()) });
            } else {
                THROW_NOT_IMPLEMENTED_ERROR("unknown entity found, unresolved function call name: '%1%'"_fmt % unit_.print(dval.mangled_name));
            }
        } else {
            throw exception("unresolved name: '%1%'"_fmt % unit_.print(dval.mangled_name));
        }
#endif
    }

    void operator()(entity_identifier const& eid) const
    {
        entity const& e = unit_.eregistry().get(eid);
        e.visit(*this);
    }

    void operator()(uint64_t value) const
    {
        bvm().append_push_static_const(smart_blob{ ui64_blob_result(value) });
    }

    template <typename T>
    void operator()(T const& e) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }


    void operator()(string_literal_entity const& sle) const override
    {
        this->operator()(sle.value());
    }

    void operator()(decimal_literal_entity const& dle) const override
    {
        this->operator()(dle.value());
    }

    void operator()(pack_entity const&) const override
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }




    void operator()(functional_entity const&) const override
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    void operator()(function_entity const&) const override
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}
