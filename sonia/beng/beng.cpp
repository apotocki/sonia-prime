//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "beng.hpp"

#include "unit.hpp"
#include "parser.hpp"

#include "ast/fn_compiler_context.hpp"
#include "ast/forward_declaration_visitor.hpp"
#include "ast/declaration_visitor.hpp"

#include "vm/compiler_visitor.hpp"

namespace sonia::lang::beng::detail {

using sonia::lang::beng::vm::compiler_visitor;

class beng_impl 
    : public invokation::invokable
{
public:
    void build(fs::path const&);
    void build(string_view code);
    void run(external_environment* penv, span<string_view> args);

    beng_impl() = default;
    beng_impl(beng_impl const&) = delete;
    beng_impl& operator=(beng_impl const&) = delete;

private:
    /*
    shared_ptr<invokation::invokable> create(string_view type, string_view id) override
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    void set_property(string_view id, string_view propname, blob_result const& value) override
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    */

protected:
    void compile(lang::beng::parser_context&);
    void do_compile(vm::compiler_visitor&, function_entity&);

private:
    lang::beng::unit unit_;
    
    optional<function_entity> main_function_;
};

}

namespace sonia::lang::beng {

language::language()
    : impl_{make_shared<detail::beng_impl>()}
{

}

language::~language()
{

}

void language::run(external_environment* penv, span<string_view> args)
{
    impl_->run(penv, args);
}

void language::build(fs::path const& f)
{
    impl_->build(f);
}

void language::build(string_view code)
{
    impl_->build(code);
}

}

namespace sonia::lang::beng::detail {

using namespace sonia::lang::beng;

void beng_impl::build(fs::path const& f)
{
    parser_context parser{ unit_ };
    parser.parse(f);
    compile(parser);
}

void beng_impl::build(string_view code)
{
    lang::beng::parser_context parser{ unit_ };
    parser.parse(code);
    compile(parser);
}

void beng_impl::compile(lang::beng::parser_context & pctx)
{
    // main context
    fn_compiler_context ctx{ unit_, qname{} };
    // main argument: [string]
    auto&& ve = ctx.new_position_parameter(0, beng_vector_t{ beng_string_t{} });
    ve.set_index(-1); // first parameter

    // retrieve forward declarations
    forward_declaration_visitor fdvis{ ctx };
    for (auto & d : pctx.type_declarations()) {
        apply_visitor(fdvis, d);
    }
    
    // treat types
    for (type_entity * pte : fdvis.types) {
        pte->treat(ctx);
    }

    declaration_visitor dvis{ ctx };
    for (auto & d : pctx.generic_declarations()) {
        apply_visitor(dvis, d);
    }
    ctx.finish_frame();

    // expression tree to vm script
    vm::compiler_visitor vmcvis{ unit_ };

    // at first compile all functions
    unit_.eregistry().traverse([this, &vmcvis](qname_view name, entity& e) {
        if (auto fe = dynamic_cast<function_entity*>(&e); fe && !fe->is_inline()) {
            do_compile(vmcvis, *fe);
        }
        return true;
    });

    // main
    function_signature main_sig{ };
    main_sig.position_parameters().emplace_back(beng_vector_t{ beng_string_t{} });
    main_function_.emplace(qname{}, std::move(main_sig));
    main_function_->body = std::move(ctx.expressions());
    do_compile(vmcvis, *main_function_);
}

void beng_impl::do_compile(vm::compiler_visitor & vmcvis, function_entity & fe)
{
    size_t param_count = fe.signature().parameters_count() + fe.captured_variables.size();

    auto& bvm = unit_.bvm();
    // at first return code
    size_t return_address = bvm.get_ip();
    if (!fe.is_void()) {
        bvm.append_fset(-static_cast<intptr_t>(param_count));
        bvm.append_truncatefp(-static_cast<intptr_t>(param_count) + 1);
    } else {
        bvm.append_truncatefp(-static_cast<intptr_t>(param_count));
    }
    //if (param_count) 
    bvm.append_popfp();
    bvm.append_ret();
    vmcvis.local_return_address = return_address;

    size_t address = bvm.get_ip();
    if (!fe.is_defined()) {
        fe.set_address(address);
    } else if (fe.is_variable_index()) {
        size_t varaddress = fe.get_address();
        bvm.set_at_stack(varaddress, smart_blob{ui64_blob_result(address)});
        fe.set_address(address); // update for fututre direct calls
    }

    //if (param_count) {
    bvm.append_pushfp(); // for accessing function arguments and local variables
    //}
    for (auto const& e : fe.body) {
        apply_visitor(vmcvis, e);
    }
    vmcvis.local_return_address = nullopt;
    bvm.append_jmp(return_address);
}

void beng_impl::run(external_environment* penv, span<string_view> args)
{
    vm::context ctx{ unit_.bvm(), penv };
    for (string_view arg : args) {
        smart_blob argblob{ string_blob_result(arg) };
        argblob.allocate();
        ctx.stack_push(std::move(argblob));
    }
    if (!args.empty()) {
        ctx.stack_push(ui64_blob_result(args.size()));
        ctx.arrayify();
    }
    unit_.bvm().run(ctx, main_function_->get_address());
}

}
