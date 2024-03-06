//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <boost/unordered_set.hpp>

#include "sonia/shared_ptr.hpp"

#include "procedure_entity.hpp"
#include "semantic.hpp"

#include <ranges>

//#include "sonia/utility/functional/hash/span.hpp"
namespace sonia::lang::beng {

class type_entity : public procedure_entity
{
public:
    explicit type_entity(qname_type name) : procedure_entity{ std::move(name) } {}

    bool find(compiler_context&,
        span<const expression_t> positioned_args,
        span<const std::tuple<annotated_identifier, expression_t>> named_args,
        std::vector<semantic_expression_type>& result, beng_generic_type & rtype) const override;

    /*
    struct function_descriptor
    {
        shared_ptr<function_t> fn;
        std::vector<identifier> param_names;

        friend inline bool operator==(function_descriptor const& l, function_descriptor const& r)
        {
            return range_equal{}(l.param_names, r.param_names);
        }

        friend inline bool operator==(function_descriptor const& l, span<const identifier> r)
        {
            return range_equal{}(l.param_names, r);
        }

        friend inline bool operator==(span<const identifier> const& l, function_descriptor const& r)
        {
            return range_equal{}(l, r.param_names);
        }

        friend inline size_t hash_value(function_descriptor const& v)
        {
            return hasher{}(v.param_names);
        }
    };
    */
private:
    /*
    struct f_hasher
    {
        inline size_t operator()(shared_ptr<concrete_invoke_function> const& p) const noexcept { return hash_value(*p); }
    };

    struct f_equal_to
    {
        inline bool operator()(shared_ptr<concrete_invoke_function> const& l, shared_ptr<concrete_invoke_function> const& r) const noexcept { return *l == *r; }
        inline bool operator()(span<const identifier> l, shared_ptr<concrete_invoke_function> const& r) const noexcept { return l == *r; }
    };
    */
    //mutable boost::unordered_set<function_descriptor, hasher> funcions_;
};

// parameters: {argname, argvalue} ... pair_count
//class object_constructor_function : public function_t
//{
//public:
//
//};

}

#include "expression_visitor.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia::lang::beng {

bool type_entity::find(compiler_context& ctx,
    span<const expression_t> positioned_args,
    span<const std::tuple<annotated_identifier, expression_t>> named_args,
    std::vector<semantic_expression_type>& result,
    beng_generic_type & rtype) const
{
    if (!positioned_args.empty()) {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    
    size_t initial_result_sz = result.size();
    EXCEPTIONAL_SCOPE_EXIT([&result, initial_result_sz]() { result.resize(initial_result_sz); });

    procedure_signature const& sig = signatures.back();
    size_t posargpos = 0;

    for (auto const& narg : named_args) {
        auto const& argname = std::get<0>(narg);
        auto it = std::ranges::lower_bound(sig.named_arguments, argname.id, [](auto const& l, auto const& r) { return l < r; }, [](auto const& v) { return v.first; });
        if (it == sig.named_arguments.end() || it->first != argname.id) {
            throw exception("%1%(%2%,%3%): parameter `%4%` of `%5%` is not found"_fmt %
                argname.location.resource % argname.location.line % argname.location.column %
                ctx.u().print(argname.id) % ctx.u().print(name()));
        }
        expression_visitor evis{ ctx, result, &it->second };
        apply_visitor(evis, std::get<1>(narg));
        result.emplace_back(semantic::push_value{ ctx.u().as_u32string(argname.id) });
    }
    result.emplace_back(semantic::push_value{ decimal{ named_args.size() } });
    result.emplace_back(semantic::push_value{ ctx.u().as_u32string(name()) });
    result.emplace_back(semantic::invoke_function{ this, 2 * ((uint32_t)named_args.size()) + 1 });
    rtype = beng_object_t{ this->name() };
    return true;

    // ids: check uniqueness?
    //std::ranges::sort(std::views::zip(ids, args_exprs), {}, [](auto const& pair) { return std::get<0>(pair); });
    //uint32_t argcount = (uint32_t)ids.size();
    /*
    auto it = funcions_.find(span{ ids }, hasher{}, std::equal_to<>{});
    if (it == funcions_.end()) {
        auto fn = make_shared<implemented_function>();
        for (auto argit = ids.rbegin(), argeit = ids.rend(); argit != argeit; ++argit) {
            
        }
        for (identifier argid : ids) {
            fn->body.emplace_back(semantic::push_value{ ctx.u().as_u32string() });
        }

        ctx.u().put_function(fn);
        function_descriptor fd;
        fd.param_names = std::move(ids);
        fd.fn = std::move(fn);
        it = funcions_.insert(it, std::move(fd));
    }

    semantic::invoke_function inv_fn{ it->fn.get(), argcount };
    args_exprs.emplace_back(std::move(inv_fn));
    result.emplace_back(std::move(args_exprs), beng_object_t{ name() });
    */
}

}
