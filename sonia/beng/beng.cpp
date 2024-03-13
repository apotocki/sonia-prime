//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "beng.hpp"

#include "unit.hpp"
#include "parser.hpp"

#include "ast/compiler_context.hpp"
#include "ast/forward_declaration_visitor.hpp"
#include "ast/declaration_visitor.hpp"

#include "vm/beng_vm.hpp"
#include "vm/compiler_visitor.hpp"

namespace sonia::beng::detail {

class beng_impl 
    : public invokation::invokable
{
public:
    void build(fs::path const&);
    void build(string_view code);
    void run(external_environment* penv);

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
    void compile(lang::beng::parser_context const&);

private:
    lang::beng::unit unit_;
    vm::virtual_stack_machine bvm_;
    optional<size_t> main_address_;
};

}

namespace sonia::beng {

language::language()
    : impl_{make_shared<detail::beng_impl>()}
{

}

language::~language()
{

}

void language::run(external_environment* penv)
{
    impl_->run(penv);
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

namespace sonia::beng::detail {

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

void beng_impl::compile(lang::beng::parser_context const& pctx)
{
    using namespace lang::beng;

    auto ds = pctx.declarations();
    compiler_context ctx{ unit_ };

    // treat forward declarations
    forward_declaration_visitor fdvis{ ctx };
    for (auto const& d : ds) {
        apply_visitor(fdvis, d);
    }

    declaration_visitor dvis{ ctx };
    for (auto const& d : ds) {
        apply_visitor(dvis, d);
    }
    ctx.finish_frame();

    // expression tree to vm script
    vm::compiler_visitor vmcvis{ unit_, bvm_ };

    // at first compile all functions
    unit_.eregistry().traverse([this, &vmcvis](qname_view name, entity& e) {
        if (auto fe = dynamic_cast<function_entity*>(&e); fe && !fe->is_inline()) {
            size_t param_count = fe->signature().parameters_count();

            // at first return code
            size_t return_address = bvm_.get_ip();
            if (!fe->is_void()) {
                bvm_.append_fset(-static_cast<intptr_t>(param_count));
                bvm_.append_truncatefp(-static_cast<intptr_t>(param_count) + 1);
            } else {
                bvm_.append_truncatefp(-static_cast<intptr_t>(param_count));
            }
            if (param_count) bvm_.append_popfp();
            bvm_.append_ret();
            vmcvis.local_return_address = return_address;

            size_t address = bvm_.get_ip();
            if (!fe->is_defined()) {
                fe->set_address(address);
            } else if (fe->is_variable_index()) {
                size_t varaddress = fe->get_address();
                bvm_.set_at_stack(varaddress, smart_blob(ui64_blob_result(address)));
                fe->set_address(address); // update for fututre direct calls
            }
            
            if (param_count) {
                bvm_.append_pushfp();
            }
            for (auto const& e : fe->body) {
                apply_visitor(vmcvis, e);
            }
            vmcvis.local_return_address = nullopt;
            bvm_.append_jmp(return_address);
        }
        return true;
    });

    // main
    size_t main_address = bvm_.get_ip();
    for (auto const& e : ctx.expressions) {
        apply_visitor(vmcvis, e);
    }
    bvm_.append_ret();
    main_address_ = main_address;
    
    // ...
    // execute script
    // ...
}

void beng_impl::run(external_environment* penv)
{
    vm::context ctx{ bvm_, penv };
    bvm_.run(ctx, *main_address_);
}

}
