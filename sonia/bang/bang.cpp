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

#include "entities/functions/function_entity.hpp"

#include "vm/compiler_visitor.hpp"

#include "sonia/utility/scope_exit.hpp"


namespace sonia::lang::bang::detail {

using sonia::lang::bang::vm::compiler_visitor;

class bang_impl 
{
public:
    //void build(fs::path const&);
    //void build(string_view code);
    void run(span<string_view> args);

    bang_impl() = default;
    bang_impl(bang_impl const&) = delete;
    bang_impl& operator=(bang_impl const&) = delete;

    void set_cout_writer(function<void(string_view)> writer) { unit_.set_cout_writer(std::move(writer)); }
    void set_environment(invocation::invocable* penv) { penv_ = penv; }
    void load(fs::path const& srcfile, span<string_view> args = {});
    void load(string_view code, span<string_view> args = {});

    smart_blob call(string_view name, span<const std::pair<string_view, const blob_result>> namedargs, span<const blob_result> args);

protected:
    void compile(lang::bang::parser_context&, declaration_set_t, span<string_view> args);
    void do_compile(vm::compiler_visitor&, function_entity&);

private:
    lang::bang::unit unit_;
    optional<fn_compiler_context> default_ctx_;
    optional<function_entity> main_function_;
    invocation::invocable* penv_ = nullptr;
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

void language::set_cout_writer(function<void(string_view)> writer)
{
    impl_->set_cout_writer(std::move(writer));
}

void language::set_environment(invocation::invocable* penv)
{
    impl_->set_environment(penv);
}

void language::load(fs::path const& srcfile, span<string_view> args)
{
    impl_->load(srcfile, args);
}

void language::load(string_view code, span<string_view> args)
{
    impl_->load(code, args);
}


smart_blob language::call(string_view name, span<const std::pair<string_view, const blob_result>> namedargs, span<const blob_result> args)
{
    return impl_->call(name, namedargs, args);
}

}

namespace sonia::lang::bang::detail {

using namespace sonia::lang::bang;

void bang_impl::load(fs::path const& f, span<string_view> args)
{
    parser_context parser{ unit_ };
    auto exp_decls = parser.parse(f);
    if (!exp_decls.has_value()) throw exception(exp_decls.error());
    compile(parser, std::move(*exp_decls), args);
}

void bang_impl::load(string_view code, span<string_view> args)
{
    lang::bang::parser_context parser{ unit_ };
    auto exp_decls = parser.parse(code);
    if (!exp_decls.has_value()) throw exception(exp_decls.error());
    compile(parser, std::move(*exp_decls), args);
}

void bang_impl::compile(lang::bang::parser_context & pctx, declaration_set_t decls, span<string_view> args)
{
    fn_compiler_context ctx{ unit_, qname{unit_.new_identifier()} };
    size_t argindex = 0;
    std::array<char, 16> argname = { '$' };
    for (string_view arg : args) {
        bool reversed = false;
        char * epos = mp::to_string(span{ &argindex, 1 }, argname.data() + 1, reversed);
        if (reversed) std::reverse(argname.data() + 1, epos);

        string_literal_entity smpl{arg};
        smpl.set_type(unit_.get_string_entity_identifier());
        entity const& argent = unit_.eregistry().find_or_create(smpl, [&smpl]() {
            return make_shared<string_literal_entity>(std::move(smpl));
        });
        identifier argid = unit_.slregistry().resolve(string_view{ argname.data(), epos });
        functional& arg_fnl = unit_.fregistry().resolve(ctx.ns() / argid);
        arg_fnl.set_default_entity(argent.id());
        //ctx.new_const_entity(string_view{ argname.data(), epos }, std::move(ent));
        ++argindex;
    }
    argname[1] = '$';
    decimal_literal_entity smpl{ argindex };
    smpl.set_type(unit_.get_decimal_entity_identifier());
    entity const& argent = unit_.eregistry().find_or_create(smpl, [&smpl]() {
        return make_shared<decimal_literal_entity>(std::move(smpl));
    });
    identifier argid = unit_.slregistry().resolve(string_view{ argname.data(), 2 });
    functional& arg_fnl = unit_.fregistry().resolve(ctx.ns() / argid);
    arg_fnl.set_default_entity(argent.id());

    

    if (!default_ctx_) {
        default_ctx_.emplace(unit_, qname{});
    }

    // separate declarations
    // retrieve forward declarations
    forward_declaration_visitor fdvis{ *default_ctx_, pctx };
    for (auto& d : decls) {
        apply_visitor(fdvis, d);
    }

    // treat types
    //for (type_entity* pte : fdvis.types) {
    //    pte->treat(ctx);
    //}

    declaration_visitor dvis{ ctx };
    for (auto& d : fdvis.decls) {
        apply_visitor(dvis, d);
    }
    ctx.finish_frame();


    ////    for (string_view arg : args) {
    ////        auto&& ve = main_ctx_->new_position_parameter(0, bang_string_t{});
    ////    }
    ////    // main argument: [string]
    ////    auto&& ve = main_ctx_->new_position_parameter(0, bang_vector_t{ bang_string_t{} });
    ////    ve.set_index(-1); // first parameter
    ////} else if (main_function_) {
    ////    main_ctx_->expressions() = std::move(main_function_->body);
    ////}
    //
    ////fn_compiler_context & ctx = *main_ctx_;

    // expression tree to vm script
    vm::compiler_visitor vmcvis{ unit_ };

    // at first compile all functions
    unit_.eregistry().traverse([this, &vmcvis](entity& e) {
        //GLOBAL_LOG_INFO() << unit_.print(e);
        if (auto fe = dynamic_cast<function_entity*>(&e); fe && !fe->is_defined()) {
            do_compile(vmcvis, *fe);
        }
        return true;
    });

    // main
    // all arguments referenced as constants => no fp prolog/epilog code
    auto& bvm = unit_.bvm();
    size_t main_address = bvm.get_ip();
    for (semantic::expression_t const& e : ctx.expressions()) {
        //GLOBAL_LOG_INFO() << "\n"sv << unit_.print(e);
        apply_visitor(vmcvis, e);
    }
    bvm.append_ret();

    // run
    vm::context vmctx{ unit_, penv_ };
    unit_.bvm().run(vmctx, main_address);

    //function_signature main_sig{ };
    //main_sig.position_parameters().emplace_back(bang_vector_t{ bang_string_t{} });
    //main_function_.emplace(entity_identifier{}, unit_.make_qname_identifier("main"sv), std::move(main_sig));
    //main_function_->body = std::move(ctx.expressions());
    //do_compile(vmcvis, *main_function_);
}

void bang_impl::do_compile(vm::compiler_visitor & vmcvis, function_entity & fe)
{
    if (!fe.is_built()) {
        fe.build(unit_);
    }
    GLOBAL_LOG_INFO() << "compiling function: " << unit_.print(fe.name());
    size_t param_count = fe.parameter_count(); // including captured_variables

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

    // replace index with real function address
    BOOST_ASSERT(fe.is_const_index());
    size_t fni = fe.get_index();
    bvm.set_const(fni, smart_blob{ ui64_blob_result(address) });
    fe.set_address(address);

    //if (!fe.is_defined()) {
    //    fe.set_address(address);
    //} else if (fe.is_const_index()) {
    //    size_t varaddress = fe.get_address();
    //    bvm.consts().at(varaddress).replace(smart_blob{ui64_blob_result(address)});
    //    fe.set_address(address); // update for future direct calls
    //}

    //if (param_count) {
    bvm.append_pushfp(); // for accessing function arguments and local variables
    //}
    for (auto const& e : fe.body()) {
        //GLOBAL_LOG_INFO() << "\n"sv << unit_.print(e);
        apply_visitor(vmcvis, e);
    }
    vmcvis.local_return_address = nullopt;
    bvm.append_jmp(return_address);
}

void bang_impl::run(span<string_view> args)
{
    vm::context ctx{ unit_, penv_ };
    for (string_view arg : args) {
        smart_blob argblob{ string_blob_result(arg) };
        argblob.allocate();
        ctx.stack_push(std::move(argblob));
    }
    if (!args.empty()) {
        ctx.stack_push(ui64_blob_result(args.size()));
        ctx.arrayify();
    } else {
        ctx.stack_push(nil_blob_result());
    }
    THROW_NOT_IMPLEMENTED_ERROR("bang_impl::run");
#if 0
    unit_.bvm().run(ctx, main_function_->get_address());
#endif
}

smart_blob bang_impl::call(string_view fnsig, span<const std::pair<string_view, const blob_result>> namedargs, span<const blob_result> args)
{
    smart_blob result;
    vm::context ctx{ unit_, penv_ };
    for (blob_result const& arg : args) {
        ctx.stack_push(smart_blob(arg));
    }
    qname_identifier qid = unit_.get_function_entity_identifier(fnsig);
    //span<qname_identifier> fnsig{};
    //identifier mandled_sig_id = unit_.piregistry().resolve(fnsig);
    //qname fnqn = qname{unit_.slregistry().resolve(name)} + mandled_sig_id;
    //qname_identifier qid = unit_.qnregistry().resolve(fnqn);
    THROW_NOT_IMPLEMENTED_ERROR("bang_impl::call");
#if 0
    auto ent = unit_.eregistry().find(qid);
    auto fnent = dynamic_pointer_cast<function_entity>(ent);
    if (!fnent) {
        throw exception("function '%1%' is not found"_fmt % fnsig);
    }
    if (fnent->signature().parameters_count() != args.size() + namedargs.size()) {
        throw exception("function '%1%' error: wrong number of arguments, expected: %2%, provided: %3%"_fmt % fnsig %
            fnent->signature().parameters_count() % (args.size() + namedargs.size()));
    }
    unit_.bvm().run(ctx, fnent->get_address());
    if (fnent->signature().fn_type.result != bang_tuple_t()) {
        result.replace(std::move(ctx.stack_back()));
        ctx.stack_pop();
    }
    return result;
#endif
}

//void bang_impl::set_cout_writer(function<void(string_view)> writer)
//{
//    unit_.set_cout_writer(std::move(writer));
//}

}
