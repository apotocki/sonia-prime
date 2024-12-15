//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "bang_vm.hpp"

#include <boost/container/small_vector.hpp>

#include "sonia/utility/scope_exit.hpp"
#include "sonia/mp/integer_view.hpp" 

#include "sonia/utility/invocation/functor_object.hpp"

namespace sonia::lang::bang {

class function_invoker : public invocation::functor_object
{
    uint64_t address_ : 30;
    uint64_t param_cnt_ : 16;
    uint64_t is_void_ : 1;
    uint64_t is_static_variable_index_ : 1;
    unit & unit_;
    invocation::invocable* penv_;
    smart_blob capture_;
    small_string name_; // for describing purposes only, it's @XXX for lambdas

public:
    explicit function_invoker(size_t address, bool is_vidx, size_t paramcnt, bool is_void, smart_blob capture,
        unit& u, invocation::invocable* pev, small_string name)
            : address_{ address }
            , param_cnt_{ paramcnt }, is_void_ { is_void }
            , is_static_variable_index_{ is_vidx ? 1u : 0 }
            , capture_{ std::move(capture) }
            , unit_{u}, penv_{ pev }, name_ { std::move(name) }
    {}

    string_view name() const { return name_; }

    size_t address(vm::context const& ctx) const
    {
        if (is_static_variable_index_) {
            return ctx.const_at(address_).as<size_t>();
        } else {
            return address_;
        }
    }

    size_t call(vm::context & ctx)
    {
        // capture: nil OR array of refs OR ref
        if (!capture_.is_nil()) {
            auto cnt = capture_.size_of<blob_result>();
            if (cnt == 1) {
                BOOST_ASSERT(is_ref(capture_->type) || capture_->type == blob_type::object);
                ctx.stack_push(capture_);
            } else {
                for (auto const& b : span{ capture_.data_of<blob_result>(), cnt }) {
                    BOOST_ASSERT(is_ref(b.type) || b.type == blob_type::object);
                    ctx.stack_push(b);
                }
            }
        }
        return address(ctx);
    }

    smart_blob operator()(std::span<const blob_result> args) override
    {
        if (args.size() != param_cnt_) {
            throw exception("fn invocation error: wrong number of arguments: %1%, expected: %2%"_fmt % args.size() % (uint64_t)param_cnt_);
        }
        vm::context ctx(unit_, penv_);
        for (blob_result const& arg: args) {
            ctx.stack_push(arg);
        }
        size_t procaddress = call(ctx);
        unit_.bvm().run(ctx, procaddress);
        if (is_void_) {
            return nil_blob_result();
        } else {
            return ctx.stack_back();
        }
    }
};

vm::context::context(unit& u, invocation::invocable* penv)
    : unit_{ u }, vm_{ u.bvm() }, penv_{ penv }
{}

bool vm::context::is_true(variable_type const& v) const noexcept
{
    return v.as<bool>();
    //blob_result const* br = &v.get();
    //while (br->type == blob_type::blob_reference) {
    //    br = data_of<blob_result>(*br);
    //}
    //if (br->type == blob_type::boolean) {
    //    return !!br->bp.ui8value;
    //}
    //return true;
}

size_t vm::context::callp(size_t ret_address)
{
    smart_blob p = stack_back();
    if (is_basic_integral(p->type)) {
        size_t address = p.as<size_t>();
        call_stack_push(ret_address);
        stack_pop();
        return address;
    }
    decltype(auto) ftor = p.as<invocation::functor_object>();
    int64_t sigdescr = stack_back(1).as<int64_t>(); // (the number of args + 1) [ * (-1) if no result ]
    stack_pop(2);
    if (auto* pfinv = dynamic_cast<function_invoker*>(&ftor); pfinv) {
        call_stack_push(ret_address);
        return pfinv->call(*this);
    }
    // external functor
    auto sp = stack_span(0 /*2*/, std::abs(sigdescr) - 1);
    static_assert(sizeof(variable_type) == sizeof(blob_result));
    auto result = ftor(span{ (blob_result const*)sp.data(), sp.size() });
    stack_pop(std::abs(sigdescr) - 1);
    if (sigdescr > 0) {
        stack_push(std::move(result));
    }
    return ret_address;
}

std::string vm::context::callp_describe() const
{
    smart_blob const& p = stack_back();
    if (is_basic_integral(p->type)) {
        size_t address = p.as<size_t>();
        return ("0x%1$x"_fmt % address).str();
    }
    decltype(auto) ftor = p.as<invocation::functor_object>();
    int64_t sigdescr = stack_back(1).as<int64_t>();
    if (auto* pfinv = dynamic_cast<function_invoker*>(&ftor); pfinv) {
        return ("`%1%` at 0x%2$x"_fmt % pfinv->name() % pfinv->address(*this)).str();
    }
    return "external functor";
}

void vm::context::efn(size_t fn_index)
{
    std::get<0>(vm_.efns().at(fn_index))(*this);
}

std::string vm::context::ecall_describe(size_t fn_index) const
{
    using builtin_fn = virtual_stack_machine::builtin_fn;
    switch ((builtin_fn)fn_index) {
    case builtin_fn::is_nil: return "is_nil";
    case builtin_fn::arrayify: return "arrayify";
    case builtin_fn::unpack: return "unpack";
    case builtin_fn::referify: return "referify";
    case builtin_fn::weak_create: return "weak_create";
    case builtin_fn::weak_lock: return "weak_lock";
    case builtin_fn::function_constructor: return "function_constructor";
    //case builtin_fn::extern_object_create: return "extern_object_create";
    //case builtin_fn::extern_object_set_property: return "extern_object_set_property";
    case builtin_fn::extern_object_get_property: return "extern_object_get_property";
    case builtin_fn::extern_variable_get: return "extern_variable_get";
    case builtin_fn::extern_variable_set: return "extern_variable_set";
    case builtin_fn::extern_function_call: return "extern_function_call";
    default:
        {
            auto pair = vm_.efns().at(fn_index);
            if (std::get<1>(pair).empty()) {
                return ("#%1% at 0x%2$x"_fmt % fn_index % (uintptr_t)std::get<0>(pair)).str();
            } else {
                return std::string(std::get<1>(pair));
            }
        }
    }
}

small_string vm::context::generate_object_id() const
{
    using buff_t = small_vector<char, 16>;
    buff_t tailored_name = { '_', 'i', 'd' };
    bool reversed;
    mp::to_string(std::span{ &id_counter_, 1 }, std::back_inserter(tailored_name), reversed);
    if (reversed) std::reverse(tailored_name.begin() + 3, tailored_name.end());
    ++id_counter_;
    return small_string{ tailored_name.data(), tailored_name.size() };
}

small_string vm::context::camel2kebab(string_view cc)
{
    small_vector<char, 64> buff;
    bool first = true;
    for (char c : cc) {
        if (std::isupper(c)) {
            if (!first)  {
                buff.emplace_back('-');
            }
            buff.emplace_back(std::tolower(c));
        } else {
            buff.emplace_back(c);
        }
        first = false;
    }
    return small_string{ buff.data(), buff.size() };
}

void vm::context::extern_variable_get()
{
    string_view propname = stack_back().as<string_view>();
    if (penv_) {
        stack_back().replace(penv_->get_property(propname));
    } else {
        throw exception("can't get property '%1%', no external environment was provided"_fmt % propname);
    }
}

void vm::context::extern_variable_set()
{
    SCOPE_EXIT([this] { stack_pop(); }); // assign value on stack after that
    string_view propname = stack_back().as<string_view>();
    blob_result const& propvalue = *stack_back(1);
    if (penv_) {
        penv_->set_property(propname, propvalue);
    } else {
        throw exception("can't set property '%1%' to '%2', no external environment was provided"_fmt % propname % propvalue);
    }
}

void vm::context::extern_function_call()
{
    string_view funcname = stack_back().as<string_view>();
    int64_t sigdescr = stack_back(1).as<int64_t>(); // (the number of args + 1) [ * (-1) if no result ]
    size_t argscnt = std::abs(sigdescr) - 1;
    auto sp = stack_span(2, argscnt);
    if (penv_) {
        auto result = penv_->invoke(funcname, span{ (blob_result const*)sp.data(), sp.size() });
        if (result.is_error()) {
            throw exception(result.as<string_view>());
        }
        if (sigdescr > 0) {
            stack_back(2 + argscnt).replace(std::move(result));
        }
        stack_pop(argscnt + (sigdescr > 0 ? 1 : 2));
    } else {
        throw exception("can't call extern function '%1%', no external environment was provided"_fmt % funcname);
    }
}

#if 0
void vm::context::construct_extern_object()
{
    string_view name = stack_back().as<string_view>();
    if (name.starts_with("::"sv)) {
        name = name.substr(2);
    }
    uint32_t argcount = stack_back(1).as<uint32_t>();
    
    if (!penv_) {
        SCOPE_EXIT([this, argcount] { stack_pop(argcount * 2 + 2); });
        throw exception("can't construct the object '%1%', no external environment was provided"_fmt % name);
    }

    smart_blob resobj;
    // find id
    for (uint32_t i = 0; i < argcount; ++i) {
        //GLOBAL_LOG_INFO() << stack_back(2 + 2 * i).as<string_view>();
        if (stack_back(2 + 2 * i).as<string_view>() == "id"sv) {
            string_view idval = stack_back(3 + 2 * i).as<string_view>();
            if (!idval.empty()) {
                resobj = penv_->invoke("create"sv, { string_blob_result(camel2kebab(name)), string_blob_result(idval) });
            }
        }
    }
    if (resobj.is_nil()) {
        resobj = penv_->invoke("create"sv, { string_blob_result(camel2kebab(name)), string_blob_result(generate_object_id()) });
    }

    if (resobj->type == blob_type::error) {
        throw exception(resobj.as<std::string>());
    }
    using namespace sonia::invocation;
    shared_ptr<invocable> obj = resobj.as<wrapper_object<shared_ptr<invocable>>>().value;

    for (uint32_t i = 0; i < argcount; ++i) {
        string_view arg_name = stack_back(2 + 2 * i).as<string_view>();
        if (arg_name == "id"sv) continue;
        auto const& arg_value = stack_back(3 + 2 * i);
        obj->set_property(camel2kebab(arg_name), *arg_value);
    }
    stack_pop(argcount * 2 + 2);

    return stack_push(smart_blob{ object_blob_result(obj) });
}
#endif

// type -> obj
//void vm::context::extern_object_create()
//{
//    string_view name = stack_back().as<string_view>();
//    if (name.starts_with("::"sv)) {
//        name = name.substr(2);
//        name = name.substr(2);
//    }
//    smart_blob resobj = penv_->invoke("create"sv, { string_blob_result(camel2kebab(name)) });
//    if (resobj->type == blob_type::error) {
//        throw exception(resobj.as<std::string>());
//    }
//    stack_back().replace(std::move(resobj));
//}

// (obj, propName)->value
void vm::context::extern_object_get_property()
{
    using namespace sonia::invocation;
    shared_ptr<invocable> obj = stack_back(1).as<wrapper_object<shared_ptr<invocable>>>().value;
    string_view prop_name = stack_back().as<string_view>();
    auto result = obj->get_property(camel2kebab(prop_name));
    stack_pop(2);
    stack_push(std::move(result));
}

// (obj, value, propName)->(obj, value)
//void vm::context::extern_object_set_property()
//{
//    using namespace sonia::invocation;
//    shared_ptr<invocable> obj = stack_back(2).as<wrapper_object<shared_ptr<invocable>>>().value;
//    string_view prop_name = stack_back().as<string_view>();
//    obj->set_property(camel2kebab(prop_name), *stack_back(1));
//    stack_pop();
//}

void vm::context::construct_function()
{
    int64_t sigdescr = stack_back().as<int64_t>();
    small_string name = stack_back(1).as<small_string>();
    smart_blob capture = stack_back(2);
    bool is_vidx = stack_back(3).as<bool>();
    size_t address = stack_back(4).as<size_t>();
    size_t paramcount = std::abs(sigdescr) - 1;
    bool is_void = sigdescr < 0;
    smart_blob res{ object_blob_result<function_invoker>(address, is_vidx, paramcount, is_void, std::move(capture), unit_, penv_, std::move(name)) };
    stack_pop(5);
    stack_push(std::move(res));
}

void vm::context::is_nil()
{
    auto const& val = stack_back();
    stack_push(bool_blob_result(val.is_nil() || ::is_nil(unref(*val))));
}

void vm::context::arrayify()
{
    boost::container::small_vector<blob_result, 4> elements;

    size_t argcount = stack_back().as<size_t>();
    elements.reserve(argcount);

    SCOPE_EXCEPTIONAL_EXIT([&elements]() {
        for (auto& e : elements) blob_result_unpin(&e);
    });

    for (size_t i = argcount; i > 0; --i) {
        elements.emplace_back(*stack_back(i));
        blob_result_pin(&elements.back());
    }
    smart_blob r{ array_blob_result(span{elements.data(), elements.size()}) };
    r.allocate();
    //GLOBAL_LOG_INFO() << "arrayify address: " << std::hex << r->bp.data;

    stack_pop(argcount + 1);
    stack_push(std::move(r));
}

void vm::context::unpack()
{
    smart_blob arr = std::move(stack_back());
    stack_pop();
    span<const blob_result> elems = arr.as<span<const blob_result>>();
    for (blob_result const& elem : elems) {
        stack_push(smart_blob{ std::move(const_cast<blob_result&>(elem)) });
    }
}

void vm::context::referify()
{
    variable_type& v = stack_at(stack_back().as<size_t>());
    if (!is_ref(v->type) && v->type != blob_type::object) { // objects are already reference types
        v.referify();
    }
    stack_pop(); // removing var index from stack
}

void vm::context::weak_create()
{
    using namespace sonia::invocation;
    using wrapper_object_t = wrapper_object<shared_ptr<invocable>>;
    using weak_wrapper_object_t = wrapper_object<weak_ptr<invocable>>;
    smart_blob& val = stack_back();
    if (val->type == blob_type::object) {
        object& obj = val.as<object>();
        if (auto* psobj = dynamic_cast<wrapper_object_t*>(&obj); psobj) {
            stack_push(smart_blob{object_blob_result<weak_wrapper_object_t>(psobj->value)});
        } else if (auto* pwobj = dynamic_cast<weak_wrapper_object_t*>(&obj); pwobj) { // already weak
            stack_push(val);
        } else { // weak for non-object values
            THROW_NOT_IMPLEMENTED_ERROR();
        }
        return;
    }
    THROW_NOT_IMPLEMENTED_ERROR();
}

void vm::context::weak_lock()
{
    using namespace sonia::invocation;
    using wrapper_object_t = wrapper_object<shared_ptr<invocable>>;
    using weak_wrapper_object_t = wrapper_object<weak_ptr<invocable>>;
    smart_blob& val = stack_back();
    blob_result const& urval = unref(*val);
    if (urval.type == blob_type::object) {
        object& obj = as<object>(urval);
        if (auto* pwobj = dynamic_cast<weak_wrapper_object_t*>(&obj); pwobj) { // already weak
            val.replace(smart_blob{ object_blob_result<wrapper_object_t>(pwobj->value) });
        } else if (auto* psobj = dynamic_cast<wrapper_object_t*>(&obj); psobj) { // already not weak
            // do nothing
        } else {
            THROW_NOT_IMPLEMENTED_ERROR();
        }
        return;
    }
    THROW_NOT_IMPLEMENTED_ERROR();
}

void vm::context::call_function_object()
{
    int64_t sigdescr = stack_back().as<int64_t>(); // (the number of args + 1) [ * (-1) if no result ]
    decltype(auto) ftor = stack_back(1).as<invocation::functor_object>();
    auto sp = stack_span(2, std::abs(sigdescr) - 1);
    static_assert(sizeof(variable_type) == sizeof(blob_result));
    auto result = ftor(span{(blob_result const*)sp.data(), sp.size()});
    stack_pop(std::abs(sigdescr) + 1);
    if (sigdescr > 0) {
        stack_push(std::move(result));
    }
}

virtual_stack_machine::virtual_stack_machine()
{
    set_efn((size_t)builtin_fn::is_nil, [](vm::context& ctx) { ctx.is_nil(); });
    set_efn((size_t)builtin_fn::arrayify, [](vm::context& ctx) { ctx.arrayify(); });
    set_efn((size_t)builtin_fn::unpack, [](vm::context& ctx) { ctx.unpack(); });
    set_efn((size_t)builtin_fn::referify, [](vm::context& ctx) { ctx.referify(); });
    set_efn((size_t)builtin_fn::weak_create, [](vm::context& ctx) { ctx.weak_create(); });
    set_efn((size_t)builtin_fn::weak_lock, [](vm::context& ctx) { ctx.weak_lock(); });
    set_efn((size_t)builtin_fn::function_constructor, [](vm::context& ctx) { ctx.construct_function(); });
    //set_efn((size_t)builtin_fn::extern_object_create, [](vm::context& ctx) { ctx.extern_object_create(); });
    //set_efn((size_t)builtin_fn::extern_object_set_property, [](vm::context& ctx) { ctx.extern_object_set_property(); });
    set_efn((size_t)builtin_fn::extern_object_get_property, [](vm::context& ctx) { ctx.extern_object_get_property(); });
    set_efn((size_t)builtin_fn::extern_function_call, [](vm::context& ctx) { ctx.extern_function_call(); });
    set_efn((size_t)builtin_fn::extern_variable_get, [](vm::context& ctx) { ctx.extern_variable_get(); });
    set_efn((size_t)builtin_fn::extern_variable_set, [](vm::context& ctx) { ctx.extern_variable_set(); });
    /*
    builtins_.resize((    set_efn((size_t)builtin_fn::referify, [](vm::context& ctx) { ctx.referify(); });
)builtin_fn::eof_builtin_type);
    */
}

void virtual_stack_machine::append_ecall(builtin_fn fn)
{
    base_t::append_ecall((size_t)fn);
}

size_t virtual_stack_machine::add_pooled_const(smart_blob&& value)
{
    auto it = literals_.find(*value);
    if (it == literals_.end()) {
        size_t index = base_t::add_const(std::move(value));
        it = literals_.insert(it, std::pair{ *base_t::consts()[index], index });
        GLOBAL_LOG_DEBUG() << "placing new const value: " << base_t::consts()[index] << ", at index: " << index;
    } else {
        GLOBAL_LOG_DEBUG() << "found existing value: " << value << ", at index: " << it->second;
    }
    return it->second;
}

void virtual_stack_machine::set_const(size_t index, smart_blob&& value)
{
    auto it = literals_.find(*value);
    if (it == literals_.end()) {
        literals_.insert(it, std::pair{ *value, index });
    }
    base_t::set_const(index, std::move(value));
}

void virtual_stack_machine::append_push_pooled_const(smart_blob&& value)
{
    size_t pos = add_pooled_const(std::move(value));
    append_pushc(pos);
}

}
