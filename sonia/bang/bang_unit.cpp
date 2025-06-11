//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "unit.hpp"
#include "library/library.hpp"

#include "parser.hpp"
#include "ast/fn_compiler_context.hpp"
#include "ast/declaration_visitor.hpp"
#include "ast/base_expression_visitor.hpp"

#include "vm/bang_vm.hpp"
#include "entities/internal_type_entity.hpp"
#include "entities/ellipsis/ellipsis_pattern.hpp"
#include "entities/functions/function_entity.hpp"
#include "entities/functions/create_identifier_pattern.hpp"

#include "functional/external_fn_pattern.hpp"
#include "functional/general/error_pattern.hpp"
#include "functional/general/assert_pattern.hpp"
#include "functional/general/mut_pattern.hpp"
#include "functional/general/deref_pattern.hpp"
#include "functional/general/equal_pattern.hpp"
#include "functional/general/typeof_pattern.hpp"
#include "functional/general/to_string_pattern.hpp"
#include "functional/general/negate_pattern.hpp"
#include "functional/general/is_const_pattern.hpp"

#include "entities/literals/literal_entity.hpp"
#include "entities/literals/numeric_implicit_cast_pattern.hpp"
#include "entities/literals/const_literal_implicit_cast_pattern.hpp"
#include "entities/literals/string_concat_pattern.hpp"

#include "entities/union/union_bit_or_pattern.hpp"
#include "entities/union/union_implicit_cast_pattern.hpp"

#include "entities/tuple/tuple_pattern.hpp"
#include "entities/tuple/tuple_make_pattern.hpp"
#include "entities/tuple/tuple_size_pattern.hpp"
#include "entities/tuple/tuple_get_pattern.hpp"
#include "entities/tuple/tuple_set_pattern.hpp"
#include "entities/tuple/tuple_empty_pattern.hpp"
#include "entities/tuple/tuple_head_pattern.hpp"
#include "entities/tuple/tuple_tail_pattern.hpp"
#include "entities/tuple/tuple_implicit_cast_pattern.hpp"
#include "entities/tuple/tuple_equal_pattern.hpp"
#include "entities/tuple/tuple_project_get_pattern.hpp"
#include "entities/tuple/tuple_project_size_pattern.hpp"

#include "entities/struct/struct_new_pattern.hpp"
#include "entities/struct/struct_get_pattern.hpp"
#include "entities/struct/struct_implicit_cast_pattern.hpp"

#include "entities/enum/enum_implicit_cast_pattern.hpp"

#include "entities/collections/array_implicit_cast_pattern.hpp"
#include "entities/collections/array_elements_implicit_cast_pattern.hpp"

#include "entities/metaobject/metaobject_pattern.hpp"
#include "entities/metaobject/metaobject_typeof_pattern.hpp"
#include "entities/metaobject/metaobject_head_pattern.hpp"
#include "entities/metaobject/metaobject_tail_pattern.hpp"
#include "entities/metaobject/metaobject_empty_pattern.hpp"
#include "entities/metaobject/metaobject_bit_and_pattern.hpp"

#include "semantic/expression_printer.hpp"
#include "auxiliary.hpp"

namespace sonia::lang::bang {

functional& unit::resolve_functional(qname_view qn)
{
    assert(qn.is_absolute());
    return fregistry_resolve(qn);
}

#if 0
qname_identifier unit::get_function_entity_identifier(string_view signature)
{
    using sonia::get;

    parser_context parser{ *this };
    auto decls = parser.parse_string((string_view)("extern fn %1%;"_fmt % signature).str());

    fn_compiler_context ctx{ *this, qname{} };
    auto& fndecl = get<fn_pure_t>(decls->front());
    fndecl.aname.value.set_absolute();

    THROW_NOT_IMPLEMENTED_ERROR("unit get_function_entity_identifier");
#if 0
    function_signature sig;
    sig.setup(ctx, fndecl.parameters);
    sig.normilize(ctx);
    sig.build_mangled_id(*this);

    qname fnm = fndecl.name() / sig.mangled_id;
    return qnregistry().resolve(fnm);
#endif
}
#endif

identifier unit::new_identifier()
{
    return identifier_builder_();
    //auto r = identifier_builder_();
    //auto rv = r.value;
    //return r;
}

identifier unit::make_identifier(string_view sv)
{
    return slregistry_.resolve(sv);
}

qname_identifier unit::new_qname_identifier()
{
    return fregistry_resolve(qname{ new_identifier() }).id();
}

qname_identifier unit::make_qname_identifier(string_view sv)
{
    qname qn{ slregistry_.resolve(sv) };
    return fregistry_resolve(qn).id();
}

//void unit::push_entity(shared_ptr<entity> e)
//{
//    entity_identifier eid = entity_identifier_builder_();
//    e->set_id(eid);
//    eregistry_insert(std::move(e));
//}

//size_t unit::allocate_constant_index()
//{
//    return bvm().add_const(smart_blob{});
//}

std::string unit::describe_efn(size_t fn_index) const
{
    using sonia::get;
    auto pair = bvm_->efns().at(fn_index);
    if (get<1>(pair).empty()) {
        return ("#%1% at 0x%2$x"_fmt % fn_index % (uintptr_t)get<0>(pair)).str();
    } else {
        return ("#%1% '%2%'"_fmt % fn_index % std::string(get<1>(pair))).str();
    }
}

std::pair<functional*, fn_pure_t> unit::parse_extern_fn(string_view signature)
{
    parser_context parser{ *this };
    auto decls = parser.parse_string((string_view)("extern fn ::%1%;"_fmt % signature).str());
    if (!decls.has_value()) [[unlikely]] {
        throw exception(decls.error());
    }

    fn_pure_t fndecl = sonia::get<fn_pure_t>(decls->front());

    // if the result is not defined we can not resove it (e.g. from the function body) => suppose that it is void
    if (!fndecl.result) {
        fndecl.result = annotated_entity_identifier{ this->get(builtin_eid::void_), fndecl.location() };
    }

    return { &resolve_functional(qname { fndecl.name() }), std::move(fndecl) };
}

template <std::derived_from<functional::pattern> PT>
void unit::set_const_extern(string_view signature)
{
    auto [pf, fndecl] = parse_extern_fn(signature);
    fn_compiler_context ctx{ *this, qname{} };
    auto ptrn = make_shared<PT>(*pf);
    if (auto err = ptrn->init(ctx, fndecl); err) {
        throw exception(print(*err));
    }
    pf->push(ptrn);
}

template <std::derived_from<external_fn_pattern> PT>
void unit::set_extern(string_view signature, void(*pfn)(vm::context&))
{
    auto [pf, fndecl] = parse_extern_fn(signature);
    fn_compiler_context ctx{ *this, qname{} };
    auto ptrn = make_shared<PT>(*pf, fn_identifier_counter_);
    if (auto err = ptrn->init(ctx, fndecl); err) {
        throw exception(print(*err));
    }
    pf->push(ptrn);

    // to do: mangled name
    std::ostringstream ss;
    print_to(ss, pf->name());
    ptrn->print(*this, ss);
    bvm_->set_efn(fn_identifier_counter_++, pfn, small_string{ ss.str() });
}

entity_identifier unit::set_builtin_extern(string_view signature, void(*pfn)(vm::context&))
{
    auto [pf, fndecl] = parse_extern_fn(signature);
    entity_signature sig{ pf->id() };
    fn_compiler_context ctx{ *this, qname{} };
    size_t paramnum = 0;
    for (auto const& param : fndecl.parameters) {
        auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param.name);

        auto res = apply_visitor(base_expression_visitor{ ctx, ctx.expression_store() }, *param.constraints.expression);
        if (!res) throw exception(print(*res.error()));
        auto se = res->first;
        if (!se.is_const_result) {
            if (external_name) {
                throw exception("extern function parameter '%1%' must be const"_fmt % print(external_name->value));
            } else {
                throw exception("extern function parameter $%1% must be const"_fmt % paramnum);
            }
        }
        
        if (external_name) {
            sig.emplace_back(external_name->value, se.value(), param.modifier == parameter_constraint_modifier_t::const_value_type);
        } else {
            ++paramnum;
            sig.emplace_back(se.value(), param.modifier == parameter_constraint_modifier_t::const_value_type);
        }
    }

    if (fndecl.result) {
        auto res = apply_visitor(base_expression_visitor{ ctx, ctx.expression_store() }, *fndecl.result);
        if (!res) throw exception(print(*res.error()));
        auto se = res->first;
        if (!se.is_const_result) {
            throw exception("extern function result must be const"_fmt);
        }
        sig.result.emplace(se.value(), false);
    } else {
        sig.result.emplace(get(builtin_eid::void_), false);
    }
    auto pent = make_shared<external_function_entity>(*this, qname{pf->name()}, std::move(sig), fn_identifier_counter_);
    eregistry_insert(pent);
    bvm_->set_efn(fn_identifier_counter_++, pfn, small_string{ signature });
    return pent->id;
}

#if 0
entity_identifier unit::set_builtin_extern(string_view name, void(*pfn)(vm::context&))
{
    qname qn{ make_identifier(name) };
    qname_identifier qid = fregistry_resolve(qn).id();
    entity_signature sig{ qid };
    sig.result = field_descriptor{ get(builtin_eid::any), false };
    auto pent = make_shared<external_function_entity>(*this, std::move(qn), std::move(sig), fn_identifier_counter_);
    eregistry_insert(pent);
    bvm_->set_efn(fn_identifier_counter_++, pfn, small_string{ name });
    return pent->id;
}
#endif

//variable_entity& unit::new_variable(qname_view var_qname, lex::resource_location const& loc, entity_identifier t, variable_entity::kind k)
//{
//    functional& fnl = fregistry_resolve(var_qname);
//    entity_identifier eid = fnl.default_entity();
//    if (fnl.default_entity(ctx)) {
//        throw exception(print(identifier_redefinition_error{ annotated_qname_identifier{fnl.id(), loc}, eregistry_get(eid).location() }));
//    }
//    auto ve = sonia::make_shared<variable_entity>(std::move(t), fnl.id(), k);
//    ve->set_location(loc);
//    
//    eregistry_insert(ve);
//    fnl.set_default_entity(ve->id());
//
//    return *ve;
//}



unit::~unit()
{
    eregistry_.clear();
}

void unit::setup_type(builtin_qnid bqnid, builtin_eid beid)
{
    qname_identifier qnid = get(bqnid);
    functional& some_type_fnl = fregistry_resolve(qnid);
    auto some_type_entity = make_shared<basic_signatured_entity>();
    some_type_entity->signature()->name = qnid;
    some_type_entity->signature()->result.emplace(get(builtin_eid::typename_));
    eregistry_insert(some_type_entity);
    some_type_fnl.set_default_entity(annotated_entity_identifier{ some_type_entity->id });
    builtin_eids_[(size_t)beid] = some_type_entity->id;
}

void unit::set_efn(size_t idx, qname_identifier fnq)
{
    if (vm_builtins_.size() <= idx) {
        vm_builtins_.resize(idx + 1);
    }
    vm_builtins_[idx] = std::move(fnq);
}

std::vector<char> unit::get_file_content(fs::path const& rpath, fs::path const* context)
{
    if (rpath.is_absolute()) {
        if (!fs::exists(rpath) || fs::is_directory(rpath)) {
            throw exception("can't resolve path: %1%"_fmt % rpath);
        }
        return read_file(rpath);
    }
            
    auto tested_path = context ? context->parent_path() / rpath : rpath;
    for (auto it = additional_paths_.begin();;) {
        if (fs::exists(tested_path) && fs::is_regular_file(tested_path)) {
            return read_file(tested_path);
        }
        if (it == additional_paths_.end()) {
            throw exception("can't resolve path: %1%"_fmt % rpath);
        }
        tested_path = (*it++) / rpath;
    }
}

statement_span unit::push_ast(fs::path const&, managed_statement_list&& msl)
{
    if (msl) {
        statement_span sp{ static_cast<statement_entry*>(&msl.front_entry()), static_cast<statement_entry*>(&msl.back_entry()) };
        ast_.splice_back(msl);
        BOOST_ASSERT(!msl);
        return sp;
    }
    return {};
}

void unit::store(semantic::managed_expression_list&& el)
{
    expressions_.splice_back(el);
}

template <typename OutputIteratorT, typename UndefinedFT>
OutputIteratorT unit::identifier_printer(identifier const& id, string_view prefix, OutputIteratorT oi, UndefinedFT const& uf) const
{
    if (auto const* pstr = slregistry_.resolve(id); pstr) {
        oi = std::copy(pstr->begin(), pstr->end(), std::copy(prefix.begin(), prefix.end(), std::move(oi)));
    } else if (auto sp = piregistry_.resolve(id); sp) {
        if (!sp->empty()) {
            //oi = std::copy(prefix.begin(), prefix.end(), std::move(oi));
            *oi++ = '<';
            for (auto const& qn : *sp) {
                if (&qn != &sp->front()) *oi++ = ',';
                oi = name_printer(functional_registry_.resolve(qn).name(), std::move(oi), uf);
            }
            *oi++ = '>';
        }
    } else {
        uf(id, prefix, oi);
    }
    return std::move(oi);
}

template <typename OutputIteratorT, typename UndefinedFT>
OutputIteratorT unit::name_printer(qname_view const& qn, OutputIteratorT oi, UndefinedFT const& uf) const
{
    if (!qn.empty()) {
        oi = identifier_printer(qn.front(), qn.is_absolute() ? "::"sv : ""sv, std::move(oi), uf);
    
        for (identifier const& id : qn.subspan(1)) {
            oi = identifier_printer(id, "::"sv, std::move(oi), uf);
        }
    }
    return std::move(oi);
}

std::ostream& unit::print_to(std::ostream& os, identifier const& id) const
{
    boost::container::small_vector<char, 32> result;
    identifier_printer(id, ""sv, std::back_inserter(result), [](identifier const& id, string_view prefix, auto & oi) {
        std::ostringstream ss;
        ss << prefix << "@"sv << id.value;
        auto str = ss.str();
        oi = std::copy(str.begin(), str.end(), std::move(oi));
    });
    os.write(result.data(), result.size());
    return os;
}

std::ostream& unit::print_to(std::ostream& os, entity_identifier const& id) const
{
    if (id) {
        eregistry_.get(id).print_to(os, *this);
    } else {
        os << "nil-entity"sv;
    }
    return os;
}

std::ostream& unit::print_to(std::ostream& os, entity const& e) const
{
    return e.print_to(os, *this);
}

std::ostream& unit::print_to(std::ostream& os, field_descriptor const& fd) const
{
    if (fd.name()) {
        print_to(os, fd.name()) << ": "sv;
    }
    if (fd.entity_id() == get(builtin_eid::void_)) {
        return os << "()"sv;
    } else {
        if (fd.is_const()) os << "const "sv;
        return print_to(os, fd.entity_id());
    }
}

std::ostream& unit::print_to(std::ostream& os, entity_signature const& sgn) const
{
    os << print(sgn.name);
    if (!sgn.fields().empty()) {
        os << '(';
        bool first = true;

        for (uint32_t i = 0; i < sgn.fields().size(); ++i) {
            if (first) first = false;
            else os << ", "sv;
            
            print_to(os, sgn.fields()[i]);
        }

        os <<')';
    }
    if (sgn.result && sgn.result->entity_id() != get(builtin_eid::typename_)) {
        os << "->"sv;
        print_to(os, *sgn.result);
    }
    return os;
}

std::ostream& unit::print_to(std::ostream& os, qname_view const& qn) const
{
    small_vector<char, 32> result;
    name_printer(qn, std::back_inserter(result), [](identifier const& id, string_view prefix, auto & oi) {
        std::ostringstream ss;
        ss << prefix << "@"sv << id.value;
        auto str = ss.str();
        oi = std::copy(str.begin(), str.end(), std::move(oi));
    });
    os.write(result.data(), result.size());
    return os;
}

std::ostream& unit::print_to(std::ostream& os, qname_identifier const& qid) const
{
    if (qid) {
        return print_to(os, functional_registry_.resolve(qid).name());
    }
    return os << "`uninitialized qname`"s;
}

std::ostream& unit::print_to(std::ostream& os, small_u32string const& str, bool in_quotes) const
{
    namespace cvt = boost::conversion;
    if (in_quotes) os << '`';
    (cvt::cvt_push_iterator(cvt::utf32 | cvt::utf8, std::ostreambuf_iterator(os)) << str).flush();
    if (in_quotes) os << '`';
    return os;
}

std::ostream& unit::print_to(std::ostream& os, lex::resource_location const& loc) const
{
    return os << loc.resource << '(' << loc.line << ',' << loc.column << ')';
    //return os << ("%1%(%2%,%3%)"_fmt % loc.resource % loc.line % loc.column).str();
}

struct type_printer_visitor : static_visitor<void>
{
    unit const& u_;
    std::ostringstream & ss;
    explicit type_printer_visitor(unit const& u, std::ostringstream& s) : u_{u}, ss{s} {}

    template <typename T>
    inline void operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("unit::type_printer_visitor operator()");
    }

    inline void operator()(annotated_qname const& qn) const { ss << u_.print(qn.value); }
    inline void operator()(annotated_identifier const& obj) const { ss << u_.print(obj.value); }
    //inline void operator()(bang_object_t const& obj) const { ss << u_.print(obj.id()); }
        
    inline void operator()(bang_fn_type_t const& fn) const
    {
        (*this)(fn.arg);
        ss << "->"sv;
        if (fn.result) {
            apply_visitor(*this, *fn.result);
        } else {
            ss << "()"sv;
        }
    }

    inline void operator()(named_expression_list_t const& nel) const
    {
        ss << '(';
        bool first = true;
        for (auto const& f : nel) {
            if (first) first = false; else ss << ',';
            if (auto* pname = f.name(); pname) {
                ss << u_.print(pname->value) << ": "sv;
            }
            apply_visitor(*this, f.value());
        }
        ss << ')';
    }

    template <typename FamilyT>
    inline void operator()(bang_vector<FamilyT> const& v) const
    {
        ss << '[';
        apply_visitor(*this, v.type);
        ss << ']';
    }

    template <typename FamilyT>
    inline void operator()(index_expression<FamilyT> const& ie) const
    {
        apply_visitor(*this, ie.base);
        ss << '[';
        apply_visitor(*this, ie.index);
        ss << ']';
    }

    //inline void operator()(bang_tuple_t const& tpl) const
    //{
    //    ss << '(';
    //    for (auto const& f : tpl.fields) {
    //        if (&f != &tpl.fields.front()) ss << ',';
    //        if (auto* pname = f.name(); pname) {
    //            ss << u_.print(pname->value) << ": "sv;
    //        }
    //        apply_visitor(*this, f.value());
    //    }
    //    ss << ')';
    //}

    //template <typename FamilyT>
    //inline void operator()(bang_tuple<FamilyT> const& tpl) const
    //{
    //    ss << '(';
    //    for (auto const& f : tpl.fields) {
    //        if (&f != &tpl.fields.front()) ss << ',';
    //        apply_visitor(*this, f);
    //    }
    //    ss << ')';
    //}

    //inline void operator()(bang_bunion_t const& bu) const
    //{
    //    ss << "{true: "sv;
    //    apply_visitor(*this, bu.true_type);
    //    ss << ", false: "sv;
    //    apply_visitor(*this, bu.true_type);
    //    ss << '}';
    //}

    //inline void operator()(bang_union_t const& tpl) const
    //{
    //    bool first = true;
    //    for (auto const& f : tpl) {
    //        if (!first) ss << "|"sv; else first = false;
    //        apply_visitor(*this, f);
    //    }
    //}

    template <typename FamilyT>
    inline void operator()(bang_union<FamilyT> const& tpl) const
    {
        for (auto const& f : tpl.members) {
            if (&f != &tpl.members.front()) ss << "||";
            apply_visitor(*this, f);
        }
    }
};

//std::string unit::print(bang_preliminary_type const& tp) const
//{
//    std::ostringstream ss;
//    type_printer_visitor vis{ *this, ss };
//    apply_visitor(vis, tp);
//    return ss.str();
//}

//std::string unit::print(bang_type const& tp) const
//{
//    std::ostringstream ss;
//    type_printer_visitor vis{ *this, ss };
//    apply_visitor(vis, tp);
//    return ss.str();
//}

//small_string unit::as_string(identifier const& id) const
//{
//    boost::container::small_vector<char, 32> result;
//    identifier_printer(id, ""sv, std::back_inserter(result), [](identifier const& id, string_view, auto &) {
//        throw exception("identifier '%1%' has no string representation"_fmt % id.value);
//    });
//    return { result.data(), result.size() };
//}
//
//small_string unit::as_string(entity_identifier const& id) const
//{
//    THROW_NOT_IMPLEMENTED_ERROR("unit::as_string entity_identifier");
//}
//
//small_string unit::as_string(qname_view qn) const
//{
//    boost::container::small_vector<char, 32> result;
//    name_printer(qn, std::back_inserter(result), [](identifier const& id, string_view, auto &) {
//        throw exception("identifier '%1%' has no string representation"_fmt % id.value);
//    });
//    return { result.data(), result.size() };
//}
//
//small_string unit::as_string(qname_identifier name) const
//{
//    return as_string(functional_registry_.resolve(name).name());
//}

//small_u32string unit::as_u32string(identifier const& id) const
//{
//    namespace cvt = boost::conversion;
//    if (auto const* pstr = slregistry_.resolve(id); pstr) {
//        boost::container::small_vector<char32_t, 32> result;
//        (cvt::cvt_push_iterator(cvt::utf8 | cvt::utf32, std::back_inserter(result)) << *pstr).flush();
//        return small_u32string{ result.data(), result.size() };
//    }
//    throw exception("identifier '%1%' has no string representation"_fmt % id.value);
//}



//small_u32string unit::as_u32string(qname_view name) const
//{
//    namespace cvt = boost::conversion;
//    boost::container::small_vector<char32_t, 32> result;
//    for (identifier const& id : name) {
//        if (&name.front() != &id || name.is_absolute()) {
//            result.push_back(':');
//            result.push_back(':');
//        }
//        if (auto const* pstr = slregistry_.resolve(id); pstr) {
//            (cvt::cvt_push_iterator(cvt::utf8 | cvt::utf32, std::back_inserter(result)) << *pstr).flush();
//        } else if (auto sp = piregistry_.resolve(id); sp) {
//
//
//        } else {
//            throw exception("identifier '%1%' has no string representation"_fmt % id.value);
//            //result.push_back('$');
//            //(cvt::cvt_push_iterator(cvt::utf8 | cvt::utf32, std::back_inserter(result)) << boost::lexical_cast<std::string>(id.value)).flush();
//        }
//    }
//    return small_u32string{ result.data(), result.size() };
//}


std::vector<char> unit::read_file(fs::path const& rpath)
{
    std::vector<char> result;
    result.reserve(fs::file_size(rpath));
    std::ifstream file{ rpath.string().c_str(), std::ios::binary };
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{}, std::back_inserter(result));
    return result;
}

struct expr_printer_visitor : static_visitor<void>
{
    unit const& u_;
    std::ostream& ss;
    explicit expr_printer_visitor(unit const& u, std::ostream& s) : u_{ u }, ss{ s } {}

    template <typename T>
    void operator()(annotated<T> const& ae) const
    {
        this->operator()(ae.value);
    }

    void operator()(qname const& qn) const
    {
        ss << u_.print(qn);
    }

    void operator()(qname_view qn) const
    {
        ss << u_.print(qn);
    }

    void operator()(identifier const& i) const
    {
        ss << u_.print(i);
    }

    void operator()(bool bval) const
    {
        ss << std::boolalpha << bval;
    }

    void operator()(small_string const& s) const
    {
        ss << '"' << s << '"';
    }

    void operator()(mp::integer const& i) const
    {
        ss << to_string(i);
    }

    void operator()(mp::decimal const& d) const
    {
        ss << to_string(d);
    }

    void operator()(indirect_value const& vi) const
    {
        ss << "INDIRECT(type: "sv;
        (*this)(vi.type);
        ss << ')';
    }

    void operator()(variable_reference const& vi) const
    {
        //if (vi.implicit) {
        //    ss << "IMPLICIT"sv;
        //}
        //ss << "VAR("sv << u_.print(vi.name.value) << ")"sv;
        ss << u_.print(vi.name.value);
    }

    /*
    void operator()(chained_expression_t const& c) const
    {
        ss << "CHAIN("sv;
        apply_visitor(*this, c.expression);
        ss << ": "sv;
        for (opt_chain_link_t const& l : c.chaining) {
            if (&l != &c.chaining.front()) {
                ss << ", "sv;
            }
            apply_visitor(*this, l);
        }
        ss << ")"sv;
    }
    */

    void operator()(not_empty_expression_t const& c) const
    {
        ss << "NOTEMPTY("sv;
        apply_visitor(*this, c.value);
        ss << ")"sv;
    }

    void operator()(member_expression_t const& c) const
    {
        ss << "MEMBER("sv;
        //if (c.is_object_optional) {
        //    ss << "OPT "sv;
        //}
        apply_visitor(*this, c.object);
        ss << ", "sv << u_.print(c.property) << ")"sv;
    }

    //void operator()(expression_vector_t const& ev) const
    //{
    //    ss << '[';
    //    bool first = true;
    //    for (auto const& e : ev.elements) {
    //        if (!first) ss << ", "sv;
    //        else first = false;
    //        apply_visitor(*this, e);
    //    }
    //    ss << ']';
    //}

    void operator()(function_call_t const& f) const
    {
        ss << "CALL("sv;
        apply_visitor(*this, f.fn_object);
        ss << ")(args)"sv;
    }

    //template <typename T>
    //requires(is_unary_expression<T>::value)
    //void operator()(T const& ue) const
    //{
    //    ss << "unary("sv << (int)T::op << ", "sv;
    //    apply_visitor(*this, ue.argument);
    //    ss << ')';
    //}

    void operator()(unary_expression_t const& be) const
    {
        ss << "unary("sv << (int)be.op << ", "sv;
        (*this)(be.args);
        ss << ')';
    }

    void operator()(binary_expression_t const& be) const
    {
        ss << "binary("sv << to_string(be.op) << ", "sv;
        (*this)(be.args);
        ss << ')';
    }

    //void operator()(context_identifier const& f) const
    //{
    //    ss << "CONTEXT("sv << u_.print(f.name.value) << ")"sv;
    //}

    void operator()(lambda_t const& f) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    void operator()(annotated_entity_identifier const& f) const
    {
        if (f.value == u_.get(builtin_eid::void_)) {
            ss << "void"sv;
        } else {
            ss << "ENTITY("sv << u_.print(f.value) << ')';
        }
    }

    void operator()(named_expression_list_t const& nel) const
    {
        ss << '(';
        bool is_first = true;
        for (auto const& ne: nel) {
            if (is_first) is_first = false;
            else ss << ", "sv;
            if (auto const* pname = ne.name(); pname) {
                this->operator()(pname->value);
                ss << ": "sv;
            }
            apply_visitor(*this, ne.value());
        }
        ss << ')';
    }
    
    void operator()(placeholder const&) const
    {
        ss << '_';
    }

    void operator()(array_expression_t const& ae) const
    {
        ss << '[';
        bool first = true;
        for (auto const& e : ae.elements) {
            if (!first) ss << ", "sv;
            else first = false;
            apply_visitor(*this, e);
        }
        ss << ']';
    }

    void operator()(index_expression_t const& ie) const
    {
        ss << "INDEX("sv;
        apply_visitor(*this, ie.base);
        ss << ", "sv;
        apply_visitor(*this, ie.index);
        ss << ')';
    }

    template <typename T>
    void operator()(T const& te) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    
};

std::ostream& unit::print_to(std::ostream& os, syntax_expression_t const& e) const
{
    expr_printer_visitor vis{ *this, os };
    apply_visitor(vis, e);
    return os;
}

std::ostream& unit::print_to(std::ostream& os, semantic::expression const& e) const
{
    semantic::expression_printer_visitor vis{ *this, os };
    apply_visitor(vis, e);
    return os;
}

std::ostream& unit::print_to(std::ostream& os, semantic::expression_list_t const& elist) const
{
    for (auto const& e : elist) {
        print_to(os, e);
    }
    return os;
}

std::ostream& unit::print_to(std::ostream& os, error const& err) const
{
    error_printer_visitor vis{ *this, os };
    error const* perr = &err;
    for (;;) {
        perr->visit(vis);
        auto & cause = perr->cause();
        if (!cause) break;
        perr = cause.get();
        os << "\ncaused by: "sv;
    }
    return os;
}

//functional_entity& unit::get_functional_entity(binary_operator_type bop)
//{
//    qname_identifier op_qi;
//    switch (bop) {
//    case binary_operator_type::CONCAT:
//        op_qi = make_qname_identifier("concat"sv); break;
//    case binary_operator_type::PLUS:
//        op_qi = make_qname_identifier("operator_plus"sv); break;
//    default:
//        THROW_INTERNAL_ERROR("unit::get_functional_entity error: unknown operator '%1%'"_fmt % to_string(bop));
//    }
//    THROW_NOT_IMPLEMENTED_ERROR("unit::get_functional_entity");
//#if 0
//    auto func_ent = dynamic_pointer_cast<functional_entity>(eregistry().find(op_qi));
//    if (!func_ent) {
//        THROW_INTERNAL_ERROR("unit::get_functional_entity error: operator '%1%' is not defined"_fmt % to_string(bop));
//    }
//    return *func_ent;
//#endif
//}

syntax_expression_entry& unit::push_back_expression(syntax_expression_list_t& l, syntax_expression_t && e)
{
    syntax_expression_list_t::entry_type* pentry = syntax_expression_list_entry_pool_.new_object(std::move(e));
    l.push_back(*pentry);
    return static_cast<syntax_expression_entry&>(*pentry);
}

void unit::push_back_expression(semantic::expression_list_t& l, semantic::expression_span& sp, semantic::expression&& e)
{
    semantic::expression_list_t::entry_type* pentry = semantic_expression_list_entry_pool_.new_object(std::move(e));
    if (sp) {
        l.insert(*sp.second, *pentry);
        sp.second = static_cast<semantic::expression_entry*>(pentry);
    } else {
        l.push_back(*pentry);
        sp = semantic::expression_span{ static_cast<semantic::expression_entry*>(pentry) };
    }
}

void unit::push_back_expression(semantic::expression_list_t& l, semantic::expression&& e)
{
    semantic::expression_list_t::entry_type * pentry = semantic_expression_list_entry_pool_.new_object(std::move(e));
    l.push_back(*pentry);
}

statement_entry& unit::acquire(statement&& st)
{
    return *statements_entry_pool_.new_object(std::move(st));
}

void unit::release(syntax_expression_list_t::entry_type&& e)
{
    syntax_expression_list_entry_pool_.delete_object(&e);
}

void unit::release(semantic::expression_list_t::entry_type&& e)
{
    semantic_expression_list_entry_pool_.delete_object(&e);
}

void unit::release(statement_entry_type&& e)
{
    statements_entry_pool_.delete_object(static_cast<statement_entry*>(&e));
}

functional* unit::fregistry_find(qname_view qnid)
{
    return functional_registry_.find(qnid);
}

functional& unit::fregistry_resolve(qname_identifier qnid)
{
    return functional_registry_.resolve(qnid);
}

functional& unit::fregistry_resolve(qname_view qn)
{
    return functional_registry_.resolve(qn, [this](qname_identifier qid, qname_view qnv) {
#ifdef SONIA_LANG_DEBUG
        qid.debug_name = as_string(qnv);
#endif
        return make_shared<functional>(std::move(qid), qnv);
    });
}

entity const& unit::eregistry_get(entity_identifier eid) const
{
    return eregistry_.get(eid);
}

void unit::eregistry_insert(shared_ptr<entity> e)
{
#ifdef SONIA_LANG_DEBUG
    std::ostringstream ss;
    e->print_to(ss, *this);
    e->debug_name = ss.str();
#endif
    eregistry_.insert(std::move(e));
}

identifier_entity const& unit::make_identifier_entity(identifier value)
{
    identifier_entity smpl{ value, get(builtin_eid::identifier) };
    return static_cast<identifier_entity&>(eregistry_find_or_create(smpl, [&smpl]() {
        return make_shared<identifier_entity>(std::move(smpl));
    }));
}

//qname_identifier_entity const& unit::make_qname_identifier_entity(qname_identifier value)
//{
//    qname_identifier_entity smpl{ value, get(builtin_eid::qname_identifier) };
//    return static_cast<qname_identifier_entity&>(eregistry_find_or_create(smpl, [&smpl]() {
//        return make_shared<qname_identifier_entity>(std::move(smpl));
//    }));
//}

empty_entity const& unit::make_empty_entity(entity_identifier type)
{
    empty_entity smpl{ type };
    return static_cast<empty_entity&>(eregistry_find_or_create(smpl, [&smpl]() {
        return make_shared<empty_entity>(std::move(smpl));
    }));
}

empty_entity const& unit::make_empty_entity(entity const& e)
{
    BOOST_ASSERT(e.signature() && e.signature()->result && e.signature()->result->entity_id() == get(builtin_eid::typename_));
    return make_empty_entity(e.id);
}

qname_entity const& unit::make_qname_entity(qname_view value)
{
    qname_entity smpl{ value, get(builtin_eid::qname) };
    return static_cast<qname_entity&>(eregistry_find_or_create(smpl, [&smpl]() {
        return make_shared<qname_entity>(std::move(smpl));
    }));
}

bool_literal_entity const& unit::make_bool_entity(bool value, entity_identifier type)
{
    bool_literal_entity smpl{ value, type ? type : get(builtin_eid::boolean) };
    return static_cast<bool_literal_entity&>(eregistry_find_or_create(smpl, [&smpl]() {
        return make_shared<bool_literal_entity>(std::move(smpl));
    }));
}

integer_literal_entity const& unit::make_integer_entity(mp::integer_view value, entity_identifier type)
{
    integer_literal_entity smpl{ value, type ? type : get(builtin_eid::integer) };
    return static_cast<integer_literal_entity&>(eregistry_find_or_create(smpl, [&smpl]() {
        return make_shared<integer_literal_entity>(std::move(smpl));
    }));
}

decimal_literal_entity const& unit::make_decimal_entity(mp::decimal_view value, entity_identifier type)
{
    decimal_literal_entity smpl{ value, type ? type : get(builtin_eid::decimal) };
    return static_cast<decimal_literal_entity&>(eregistry_find_or_create(smpl, [&smpl]() {
        return make_shared<decimal_literal_entity>(std::move(smpl));
    }));
}

string_literal_entity const& unit::make_string_entity(string_view value, entity_identifier type)
{
    string_literal_entity smpl{ value, type ? type : get(builtin_eid::string) };
    return static_cast<string_literal_entity&>(eregistry_find_or_create(smpl, [&smpl]() {
        return make_shared<string_literal_entity>(std::move(smpl));
    }));
}

basic_signatured_entity const& unit::make_basic_signatured_entity(entity_signature&& sig)
{
    indirect_signatured_entity smpl{ sig };
    return static_cast<basic_signatured_entity&>(eregistry_find_or_create(smpl, [&sig]() {
        return make_shared<basic_signatured_entity>(std::move(sig));
    }));
}

basic_signatured_entity const& unit::make_vector_type_entity(entity_identifier element_type)
{
    entity_signature sig{ get(builtin_qnid::vector), get(builtin_eid::typename_) };
    sig.emplace_back(get(builtin_id::element), element_type, true);
    return make_basic_signatured_entity(std::move(sig));
}

basic_signatured_entity const& unit::make_vector_entity(entity_identifier element_type, span<entity_identifier> const& values)
{
    entity_signature sig{ get(builtin_qnid::metaobject) };
    //sig.push_back(u.get(builtin_id::element), field_descriptor{ element_type, true });
    for (auto const& v : values) {
        sig.emplace_back(v, true);
    }
    entity_identifier tp = make_vector_type_entity(element_type).id;
    sig.result = field_descriptor{ tp };
    return make_basic_signatured_entity(std::move(sig));
}

basic_signatured_entity const& unit::make_array_type_entity(entity_identifier element_type, size_t sz)
{
    entity_signature sig{ get(builtin_qnid::array), get(builtin_eid::typename_) };
    sig.emplace_back(get(builtin_id::element), element_type, true);
    entity_identifier szeid = make_integer_entity((int64_t)sz).id;
    sig.emplace_back(get(builtin_id::size), szeid, true);
    return make_basic_signatured_entity(std::move(sig));
}

basic_signatured_entity const& unit::make_array_entity(entity_identifier element_type, span<entity_identifier> const& values)
{
    entity_identifier tp = make_array_type_entity(element_type, values.size()).id;
    entity_signature sig{ get(builtin_qnid::metaobject), tp };

    for (auto const& v : values) {
        sig.emplace_back(v, true);
    }
    
    return make_basic_signatured_entity(std::move(sig));
}

entity const& unit::make_union_type_entity(span<entity_identifier> const& types)
{
    if (types.empty()) {
        return get_entity(*this, get(builtin_eid::void_));
    }

    if (types.size() == 1) {
        return get_entity(*this, *types.begin());
    }

    entity_signature usig(get(builtin_qnid::union_), get(builtin_eid::typename_));
    for (entity_identifier const& eid : types) {
        usig.push_back(field_descriptor{ eid, true });
    }

    return make_basic_signatured_entity(std::move(usig));
}

unit::unit()
    : slregistry_{ identifier_builder_ }
    , piregistry_{ identifier_builder_ }
    , fn_identifier_counter_ { (size_t)virtual_stack_machine::builtin_fn::eof_type }
    , bvm_{ std::make_unique<virtual_stack_machine>() }
    , syntax_expression_list_entry_pool_{ 128, 128 }
    , semantic_expression_list_entry_pool_{ 128, 128 }
    , statements_entry_pool_{ 128, 128 }
    , ast_{ *this }
    , expressions_ { *this }
{
    //set_extern("string(const __id)"sv, &bang_print_string);

    //qname_identifier string_lid = make_qname_identifier("string");
    //functional& string_fnl = fregistry_resolve(string_lid);
    //functional::pattern p()
    //string_fnl.push();

    //identifier id_lid = slregistry().resolve("__id");

    // internal types
    //auto ie = make_shared<internal_type_entity>(make_qname_identifier("integer"));
    //integer_entity_ = ie.get();
    //eregistry_insert(std::move(ie));
#if 0
    // internal functions
    builtins_.resize((size_t)builtin_fn::eof_builtin_type);

    auto parrayify = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::arrayify);
    eregistry_insert(parrayify);
    set_efn(builtin_fn::arrayify, parrayify->name());

    auto punpack = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::unpack);
    eregistry_insert(punpack);
    set_efn(builtin_fn::unpack, punpack->name());

    auto pweak_create = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::weak_create);
    eregistry_insert(pweak_create);
    set_efn(builtin_fn::weak_create, pweak_create->name());

    auto pweak_lock = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::weak_lock);
    eregistry_insert(pweak_lock);
    set_efn(builtin_fn::weak_lock, pweak_lock->name());

    auto pce = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::extern_object_create);
    eregistry_insert(pce);
    set_efn(builtin_fn::extern_object_create, pce->name());

    auto peosp = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::extern_object_set_property);
    eregistry_insert(peosp);
    set_efn(builtin_fn::extern_object_set_property, peosp->name());

    auto peogp = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::extern_object_get_property);
    eregistry_insert(peogp);
    set_efn(builtin_fn::extern_object_get_property, peogp->name());

    auto pefc = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::extern_function_call);
    eregistry_insert(pefc);
    set_efn(builtin_fn::extern_function_call, pefc->name());

    auto ptostring = make_shared<external_function_entity>(new_qname_identifier(), fn_identifier_counter_);
    eregistry_insert(ptostring);
    strings_.emplace_back("tostring");
    set_efn(builtin_fn::tostring, ptostring->name());
    bvm_->set_efn(fn_identifier_counter_++, &bang_tostring, strings_.back());

    auto pnegate = make_shared<external_function_entity>(new_qname_identifier(), fn_identifier_counter_);
    eregistry_insert(pnegate);
    strings_.emplace_back("!");
    set_efn(builtin_fn::negate, pnegate->name());
    bvm_->set_efn(fn_identifier_counter_++, &bang_negate, strings_.back());

    //string(const __identifier)
    set_extern("print(string)"sv, &bang_print_string);
    set_extern("concat(string,string)->string"sv, &bang_concat_string);
    set_extern("operator_plus(decimal,decimal)->decimal"sv, &bang_operator_plus_decimal);
    set_extern("decimal(text: string)->decimal|()"sv, &bang_to_decimal);
#endif

    //// ids
    #define BANG_PRINT_ENUM_ASSIGN(r, data, i, elem) \
    builtin_ids_[(size_t)builtin_id::BOOST_PP_TUPLE_ELEM(2, 0, elem)] = make_identifier(BOOST_PP_TUPLE_ELEM(2, 1, elem));
    BOOST_PP_SEQ_FOR_EACH_I(BANG_PRINT_ENUM_ASSIGN, _, BANG_BUILTIN_ID_SEQ)
#undef BANG_PRINT_ENUM_ASSIGN

        //// qnids
#define BANG_PRINT_ENUM_ASSIGN(r, data, i, elem) \
    builtin_qnids_[(size_t)builtin_qnid::BOOST_PP_TUPLE_ELEM(2, 0, elem)] = make_qname_identifier(BOOST_PP_TUPLE_ELEM(2, 1, elem));
        BOOST_PP_SEQ_FOR_EACH_I(BANG_PRINT_ENUM_ASSIGN, _, BANG_BUILTIN_QNAMES_SEQ)
#undef BANG_PRINT_ENUM_ASSIGN

    // typename
    entity_signature typename_sig{ get(builtin_qnid::typename_) };
    basic_signatured_entity const& typename_entity = make_basic_signatured_entity(std::move(typename_sig));
    builtin_eids_[(size_t)builtin_eid::typename_] = typename_entity.id;
    
    // void
    entity_signature empty_tuple_sig{ get(builtin_qnid::tuple), get(builtin_eid::typename_) };
    basic_signatured_entity const& empty_tuple_ent = make_basic_signatured_entity(std::move(empty_tuple_sig));
    empty_entity const& void_entity = make_empty_entity(empty_tuple_ent);
    builtin_eids_[(size_t)builtin_eid::void_] = void_entity.id;

    // boolean, true, false
    setup_type(builtin_qnid::boolean, builtin_eid::boolean);
    builtin_eids_[(size_t)builtin_eid::true_] = make_bool_entity(true).id;
    builtin_eids_[(size_t)builtin_eid::false_] = make_bool_entity(false).id;

    // basic literal types
    setup_type(builtin_qnid::integer, builtin_eid::integer);
    setup_type(builtin_qnid::decimal, builtin_eid::decimal);
    setup_type(builtin_qnid::f16, builtin_eid::f16);
    setup_type(builtin_qnid::f32, builtin_eid::f32);
    setup_type(builtin_qnid::f64, builtin_eid::f64);
    setup_type(builtin_qnid::string, builtin_eid::string);
    setup_type(builtin_qnid::object, builtin_eid::object);
    setup_type(builtin_qnid::identifier, builtin_eid::identifier);
    setup_type(builtin_qnid::qname, builtin_eid::qname);
    setup_type(builtin_qnid::metaobject, builtin_eid::metaobject);

    // any
    auto any_entity = make_shared<basic_signatured_entity>(entity_signature{ get(builtin_qnid::any), get(builtin_eid::typename_) });
    eregistry_insert(any_entity);
    builtin_eids_[(size_t)builtin_eid::any] = any_entity->id;
    functional& any_fnl = fregistry_resolve(get(builtin_qnid::any));
    any_fnl.set_default_entity(annotated_entity_identifier{ any_entity->id });


    /////// built in patterns
    // mut(_)
    functional& mut_fnl = fregistry_resolve(get(builtin_qnid::mut));
    mut_fnl.push(make_shared<mut_pattern>());

    // operator*(type: typename)
    functional& deref_fnl = fregistry_resolve(get(builtin_qnid::deref));
    deref_fnl.push(make_shared<deref_pattern>());

    // operator...(type: typename)
    functional& ellipsis_fnl = fregistry_resolve(get(builtin_qnid::ellipsis));
    ellipsis_fnl.push(make_shared<ellipsis_pattern>());

    // error(_) -> ()
    functional& error_fnl = fregistry_resolve(get(builtin_qnid::error));
    error_fnl.push(make_shared<error_pattern>());

    // assert(...) -> ()
    functional& assert_fnl = fregistry_resolve(get(builtin_qnid::assert));
    assert_fnl.push(make_shared<assert_pattern>());

    // equal(_, _) -> bool
    functional& equal_fnl = fregistry_resolve(get(builtin_qnid::eq));
    equal_fnl.push(make_shared<tuple_equal_pattern>());
    equal_fnl.push(make_shared<equal_pattern>());

    functional& negate_fnl = fregistry_resolve(get(builtin_qnid::negate));
    negate_fnl.push(make_shared<negate_pattern>());

    functional& typeof_fnl = fregistry_resolve(get(builtin_qnid::typeof));
    typeof_fnl.push(make_shared<typeof_pattern>());

    // typeof(object: const metaobject, property: const __identifier) -> typename
    typeof_fnl.push(make_shared<metaobject_typeof_pattern>());

    functional& to_string_fnl = fregistry_resolve(get(builtin_qnid::to_string));
    to_string_fnl.push(make_shared<to_string_pattern>());

    functional& tuple_fnl = fregistry_resolve(get(builtin_qnid::tuple));
    tuple_fnl.push(make_shared<tuple_pattern>());

    functional& implicit_cast_fnl = fregistry_resolve(get(builtin_qnid::implicit_cast));
    implicit_cast_fnl.push(make_shared<struct_implicit_cast_pattern>());
    implicit_cast_fnl.push(make_shared<enum_implicit_cast_pattern>());
    implicit_cast_fnl.push(make_shared<array_implicit_cast_pattern>());
    implicit_cast_fnl.push(make_shared<array_elements_implicit_cast_pattern>());
    implicit_cast_fnl.push(make_shared<union_implicit_cast_pattern>());
    implicit_cast_fnl.push(make_shared<numeric_implicit_cast_pattern>());
    implicit_cast_fnl.push(make_shared<tuple_implicit_cast_pattern>());
    implicit_cast_fnl.push(make_shared<const_literal_implicit_cast_pattern>());

    auto union_pattern = make_shared<union_bit_or_pattern>();
    functional& bit_or_fnl = fregistry_resolve(get(builtin_qnid::bit_or));
    bit_or_fnl.push(union_pattern);

    functional& union_fnl = fregistry_resolve(get(builtin_qnid::union_));
    union_fnl.push(union_pattern);

    functional& bit_and_fnl = fregistry_resolve(get(builtin_qnid::bit_and));
    bit_and_fnl.push(make_shared<metaobject_bit_and_pattern>());

    // make_tuple(...) -> tuple(...)
    functional& make_tuple_fnl = fregistry_resolve(get(builtin_qnid::make_tuple));
    make_tuple_fnl.push(make_shared<tuple_make_pattern>());
    
    functional& get_fnl = fregistry_resolve(get(builtin_qnid::get));
    // get(self: tuple(), property: __identifier)->T;
    get_fnl.push(make_shared<tuple_get_pattern>());
    get_fnl.push(make_shared<tuple_project_get_pattern>());
    // get(self: @structure, property: __identifier)->T;
    //get_fnl.push(make_shared<struct_get_pattern>());

    functional& set_fnl = fregistry_resolve(get(builtin_qnid::set));
    set_fnl.push(make_shared<tuple_set_pattern>());

    // size(signatured_entity)->integer
    functional& sz_fnl = fregistry_resolve(get(builtin_qnid::size));
    sz_fnl.push(make_shared<tuple_size_pattern>());
    sz_fnl.push(make_shared<tuple_project_size_pattern>());

    // __id(const string) -> __identifier
    functional& idfnl = fregistry_resolve(get(builtin_qnid::idfn));
    idfnl.push(make_shared<create_identifier_pattern>());

    // metaobject(...) -> metaobject
    functional& metaobject_fnl = fregistry_resolve(get(builtin_qnid::metaobject));
    metaobject_fnl.push(make_shared<metaobject_pattern>());



    // head(metaobject) -> ???
    functional& head_fnl = fregistry_resolve(get(builtin_qnid::head));
    head_fnl.push(make_shared<metaobject_head_pattern>());
    head_fnl.push(make_shared<tuple_head_pattern>());

    // tail(metaobject) -> @metaobject
    functional& tail_fnl = fregistry_resolve(get(builtin_qnid::tail));
    tail_fnl.push(make_shared<metaobject_tail_pattern>());
    tail_fnl.push(make_shared<tuple_tail_pattern>());

    // empty(metaobject) -> bool
    functional& empty_fnl = fregistry_resolve(get(builtin_qnid::empty));
    empty_fnl.push(make_shared<metaobject_empty_pattern>());
    empty_fnl.push(make_shared<tuple_empty_pattern>());

    // is_const(_) -> const bool
    functional& is_const_fnl = fregistry_resolve(get(builtin_qnid::is_const));
    is_const_fnl.push(make_shared<is_const_pattern>());
    
    // new(type: typename $T @struct, ...) -> $T
    functional& newfnl = fregistry_resolve(get(builtin_qnid::new_));
    newfnl.push(make_shared<struct_new_pattern>());

    // Return register the string_concat pattern
    functional& string_concat_fnl = fregistry_resolve(get(builtin_qnid::string_concat));
    string_concat_fnl.push(make_shared<string_concat_pattern>());

    //fn_result_identifier_ = make_identifier("->");

    //eq_qname_identifier_ = make_qname_identifier("==");
    //functional& eq_fnl = fregistry_resolve(eq_qname_identifier_);
    //eq_fnl.push(make_shared<eq_pattern>());

    builtin_eids_[(size_t)builtin_eid::arrayify] = set_builtin_extern("__arrayify()->any"sv, &bang_arrayify);
    builtin_eids_[(size_t)builtin_eid::array_tail] = set_builtin_extern("__array_tail()->any"sv, &bang_array_tail);
    builtin_eids_[(size_t)builtin_eid::array_at] = set_builtin_extern("__array_at()->any"sv, &bang_array_at);
    builtin_eids_[(size_t)builtin_eid::equal] = set_builtin_extern("__equal(any, any)->bool"sv, &bang_any_equal);
    builtin_eids_[(size_t)builtin_eid::assert] = set_builtin_extern("__assert(any)"sv, &bang_assert);
    builtin_eids_[(size_t)builtin_eid::to_string] = set_builtin_extern("__to_string(any)->string"sv, &bang_tostring);
    builtin_eids_[(size_t)builtin_eid::negate] = set_builtin_extern("__negate(any)->bool"sv, &bang_negate);
    builtin_eids_[(size_t)builtin_eid::concat] = set_builtin_extern("__concat(any)->string"sv, &bang_concat);
    builtin_eids_[(size_t)builtin_eid::error] = set_builtin_extern("__error(string)"sv, &bang_error);
    //set_const_extern<to_string_pattern>("size(const metaobjct))->integer"sv);

    //set_extern<external_fn_pattern>("__error(mut string)"sv, &bang_error);
    set_extern<external_fn_pattern>("__print(..., integer)"sv, &bang_print_string);
    //set_extern("implicit_cast(to: typename string, _)->string"sv, &bang_tostring);
    //set_const_extern<to_string_pattern>("to_string(const __identifier)->string"sv);
    //set_extern<external_fn_pattern>("to_string(_)->string"sv, &bang_tostring);
    //set_extern<external_fn_pattern>("implicit_cast(mut integer)->decimal"sv, &bang_int2dec);
    //set_extern<external_fn_pattern>("implicit_cast(mut integer)->float"sv, &bang_int2flt);
    set_extern<external_fn_pattern>("create_extern_object(mut string)->object"sv, &bang_create_extern_object);
    //set_extern<external_fn_pattern>("set(self: object, property: const __identifier, any)"sv, &bang_set_object_property);
    set_extern<external_fn_pattern>("set(self: mut object, property: mut string, mut _)->object"sv, &bang_set_object_property);

    //set_extern("string(any)->string"sv, &bang_tostring);
    //set_extern<external_fn_pattern>("assert(bool)"sv, &bang_assert);

    // temporary
    
    //set_extern<external_fn_pattern>("negate(mut _)->bool"sv, &bang_negate);
    set_extern<external_fn_pattern>("__plus(mut integer, mut integer)->integer"sv, &bang_operator_plus_integer);
    set_extern<external_fn_pattern>("__plus(mut decimal, mut decimal)->decimal"sv, &bang_operator_plus_decimal);
}

}
