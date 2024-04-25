//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "bang.hpp"

#include "unit.hpp"
#include "parser.hpp"

#include "ast/fn_compiler_context.hpp"
#include "ast/forward_declaration_visitor.hpp"
#include "ast/declaration_visitor.hpp"

#include "vm/compiler_visitor.hpp"

#include "sonia/utility/scope_exit.hpp"

namespace sonia::lang::bang::detail {

using sonia::lang::bang::vm::compiler_visitor;

class bang_impl 
{
public:
    void build(fs::path const&);
    void build(string_view code);
    void run(invocation::invocable* penv, span<string_view> args);

    bang_impl() = default;
    bang_impl(bang_impl const&) = delete;
    bang_impl& operator=(bang_impl const&) = delete;

protected:
    void compile(lang::bang::parser_context&, declaration_set_t);
    void do_compile(vm::compiler_visitor&, function_entity&);

private:
    lang::bang::unit unit_;
    optional<function_entity> main_function_;
};

}

namespace sonia::lang::bang {

language::language()
    : impl_{make_shared<detail::bang_impl>()}
{

}

language::~language()
{

}

void language::run(invocation::invocable* penv, span<string_view> args)
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

namespace sonia::lang::bang::detail {

using namespace sonia::lang::bang;

void bang_impl::build(fs::path const& f)
{
    parser_context parser{ unit_ };
    auto exp_decls = parser.parse(f);
    if (!exp_decls.has_value()) throw exception(exp_decls.error());
    compile(parser, std::move(*exp_decls));
}

void bang_impl::build(string_view code)
{
    lang::bang::parser_context parser{ unit_ };
    auto exp_decls = parser.parse(code);
    if (!exp_decls.has_value()) throw exception(exp_decls.error());
    compile(parser, std::move(*exp_decls));
}

void bang_impl::compile(lang::bang::parser_context & pctx, declaration_set_t decls)
{
    // main context
    fn_compiler_context ctx{ unit_, qname{} };
    // main argument: [string]
    auto&& ve = ctx.new_position_parameter(0, bang_vector_t{ bang_string_t{} });
    ve.set_index(-1); // first parameter

    // separate declarations
    // retrieve forward declarations
    forward_declaration_visitor fdvis{ ctx, pctx };
    for (auto & d : decls) {
        apply_visitor(fdvis, d);
    }
    
    // treat types
    for (type_entity * pte : fdvis.types) {
        pte->treat(ctx);
    }

    declaration_visitor dvis{ ctx };
    for (auto & d : fdvis.decls) {
        apply_visitor(dvis, d);
    }
    ctx.finish_frame();

    // expression tree to vm script
    vm::compiler_visitor vmcvis{ unit_ };

    // at first compile all functions
    unit_.eregistry().traverse([this, &vmcvis](qname_view name, entity& e) {
        if (auto fe = dynamic_cast<function_entity*>(&e); fe) {
            do_compile(vmcvis, *fe);
        }
        return true;
    });

    // main
    function_signature main_sig{ };
    main_sig.position_parameters().emplace_back(bang_vector_t{ bang_string_t{} });
    main_function_.emplace(qname{}, std::move(main_sig));
    main_function_->body = std::move(ctx.expressions());
    do_compile(vmcvis, *main_function_);
}

void bang_impl::do_compile(vm::compiler_visitor & vmcvis, function_entity & fe)
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
    } else if (fe.is_static_variable_index()) {
        size_t varaddress = fe.get_address();
        bvm.statics().at(varaddress).replace(smart_blob{ui64_blob_result(address)});
        fe.set_address(address); // update for future direct calls
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

void bang_impl::run(invocation::invocable* penv, span<string_view> args)
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
