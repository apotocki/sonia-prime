//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "unit.hpp"
#include "library/library.hpp"

#include "parser.hpp"
#include "ast/fn_compiler_context.hpp"
#include "ast/declaration_visitor.hpp"

#include "vm/bang_vm.hpp"
#include "entities/internal_type_entity.hpp"
#include "entities/ellipsis/ellipsis_pattern.hpp"
#include "entities/functions/function_entity.hpp"
#include "entities/functions/external_fn_pattern.hpp"
#include "entities/functions/create_identifier_pattern.hpp"
#include "entities/functions/extern/to_string_pattern.hpp"
#include "entities/tuple/make_tuple_pattern.hpp"
#include "entities/tuple/tuple_get_pattern.hpp"
#include "entities/struct/new_struct_pattern.hpp"
#include "entities/struct/struct_get_pattern.hpp"
#include "entities/metaobject/metaobject_pattern.hpp"
#include "entities/metaobject/typeof_pattern.hpp"
#include "semantic/expression_printer.hpp"

namespace sonia::lang::bang {

functional& unit::resolve_functional(qname_view qn)
{
    assert(qn.is_absolute());
    return fregistry().resolve(qn);
}

qname_identifier unit::get_function_entity_identifier(string_view signature)
{
    using sonia::get;

    parser_context parser{ *this };
    auto decls = parser.parse_string((string_view)("extern fn %1%;"_fmt % signature).str());

    fn_compiler_context ctx{ *this, qname{} };
    auto& fndecl = get<fn_pure>(decls->front());
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
    return fregistry().resolve(qname{ new_identifier() }).id();
}

qname_identifier unit::make_qname_identifier(string_view sv)
{
    qname qn{ slregistry_.resolve(sv) };
    return functional_registry_.resolve(qn).id();
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

std::pair<functional*, fn_pure> unit::parse_extern_fn(string_view signature)
{
    parser_context parser{ *this };
    auto decls = parser.parse_string((string_view)("extern fn ::%1%;"_fmt % signature).str());
    if (!decls.has_value()) [[unlikely]] {
        throw exception(decls.error());
    }

    fn_pure fndecl = sonia::get<fn_pure>(decls->front());

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
    auto ptrn = make_shared<PT>(ctx, *pf, fndecl);
    pf->push(ptrn);
}

template <std::derived_from<external_fn_pattern> PT>
void unit::set_extern(string_view signature, void(*pfn)(vm::context&))
{
    auto [pf, fndecl] = parse_extern_fn(signature);
    fn_compiler_context ctx{ *this, qname{} };
    auto ptrn = make_shared<PT>(ctx, *pf, fndecl, fn_identifier_counter_);
    pf->push(ptrn);

    // to do: mangled name
    std::ostringstream ss;
    ss << print(pf->name());
    ptrn->print(*this, ss);
    bvm_->set_efn(fn_identifier_counter_++, pfn, small_string{ ss.str() });
}

entity_identifier unit::set_builtin_extern(string_view name, void(*pfn)(vm::context&))
{
    qname qn{ make_identifier(name) };
    qname_identifier qid = fregistry().resolve(qn).id();
    entity_signature sig{ qid };
    sig.set_result(field_descriptor{ get(builtin_eid::any), false });
    auto pent = make_shared<external_function_entity>(*this, std::move(qn), std::move(sig), fn_identifier_counter_);
    eregistry_insert(pent);
    bvm_->set_efn(fn_identifier_counter_++, pfn, small_string{ name });
    return pent->id();
}

//variable_entity& unit::new_variable(qname_view var_qname, lex::resource_location const& loc, entity_identifier t, variable_entity::kind k)
//{
//    functional& fnl = fregistry().resolve(var_qname);
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

void unit::setup_type(string_view type_name, qname_identifier& qnid, entity_identifier& eid)
{
    qnid = make_qname_identifier(type_name);
    functional& some_type_fnl = fregistry().resolve(qnid);
    auto some_type_entity = make_shared<basic_signatured_entity>(get(builtin_eid::typename_), entity_signature{ qnid });
    eregistry_insert(some_type_entity);
    if (auto err = some_type_fnl.set_default_entity(annotated_entity_identifier{ some_type_entity->id() }); err) {
        throw exception(print(*err));
    }
    eid = some_type_entity->id();
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
    oi = identifier_printer(qn.front(), qn.is_absolute() ? "::"sv : ""sv, std::move(oi), uf);
    for (identifier const& id : qn.subspan(1)) {
        oi = identifier_printer(id, "::"sv, std::move(oi), uf);
    }
    return std::move(oi);
}

std::string unit::print(identifier const& id) const
{
    boost::container::small_vector<char, 32> result;
    identifier_printer(id, ""sv, std::back_inserter(result), [](identifier const& id, string_view prefix, auto & oi) {
        std::ostringstream ss;
        ss << prefix << "@"sv << id.value;
        auto str = ss.str();
        oi = std::copy(str.begin(), str.end(), std::move(oi));
    });
    return { result.data(), result.data() + result.size() };
}

std::string unit::print(entity_identifier const& id) const
{
    std::ostringstream ss;
    if (id) {
        eregistry_.get(id).print_to(ss, *this);
    } else {
        ss << "nil-entity"sv;
    }
    return ss.str();
}

std::string unit::print(entity const& e) const
{
    std::ostringstream ss;
    e.print_to(ss, *this);
    return ss.str();
}

std::string unit::print(entity_signature const& sgn) const
{
    std::ostringstream ss;
    ss << print(sgn.name());
    if (!sgn.positioned_fields().empty() || !sgn.named_fields().empty()) {
        ss << '(';
        bool first = true;

        //auto name_it = sgn.names().begin();
        for (auto const& [name, fd] : sgn.named_fields()) {
            if (first) first = false;
            else ss << ", "sv;
            ss << print(name) << ": "sv;
            if (fd.is_const()) ss << "const "sv;
            ss << print(fd.entity_id());
        }
        for (auto const& fd : sgn.positioned_fields()) {
            if (first) first = false;
            else ss << ", "sv;
            if (fd.is_const()) ss << "const "sv;
            ss << print(fd.entity_id());
        }

        ss << ")";
    }
    return ss.str();
}

std::string unit::print(qname_view qn) const
{
    small_vector<char, 32> result;
    name_printer(qn, std::back_inserter(result), [](identifier const& id, string_view prefix, auto & oi) {
        std::ostringstream ss;
        ss << prefix << "@"sv << id.value;
        auto str = ss.str();
        oi = std::copy(str.begin(), str.end(), std::move(oi));
    });
    return { result.data(), result.data() + result.size() };
}

std::string unit::print(qname_identifier qid) const
{
    if (qid) {
        return print(functional_registry_.resolve(qid).name());
    }
    return "`uninitialized qname`"s;
}

struct type_printer_visitor : static_visitor<void>
{
    unit const& u_;
    std::ostringstream & ss;
    explicit type_printer_visitor(unit const& u, std::ostringstream& s) : u_{u}, ss{s} {}

    inline void operator()(bang_any_t) const { ss << "any"sv; }
    //inline void operator()(bang_bool_t) const { ss << "bool"sv; }
    //inline void operator()(bang_int_t) const { ss << "int"sv; }
    //inline void operator()(bang_float_t) const { ss << "float"sv; }
    //inline void operator()(bang_decimal_t) const { ss << "decimal"sv; }
    //inline void operator()(bang_string_t) const { ss << "string"sv; }
    inline void operator()(annotated_qname const& qn) const { ss << u_.print(qn.value); }
    inline void operator()(annotated_identifier const& obj) const { ss << u_.print(obj.value); }
    inline void operator()(bang_object_t const& obj) const {
        ss << u_.print(obj.id());
    }
        
    template <typename TupleT, typename FamilyT>
    inline void operator()(bang_fn_base<TupleT, FamilyT> const& fn) const
    {
        (*this)(fn.arg);
        ss << "->"sv;
        apply_visitor(*this, fn.result);
    }

    template <typename FamilyT>
    inline void operator()(bang_vector<FamilyT> const& v) const
    {
        ss << '[';
        apply_visitor(*this, v.type);
        ss << ']';
    }

    template <typename FamilyT>
    inline void operator()(bang_array<FamilyT> const& arr) const
    {
        apply_visitor(*this, arr.type);
        ss << '[' << arr.size << ']';
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

    template <typename FamilyT>
    inline void operator()(bang_tuple<FamilyT> const& tpl) const
    {
        ss << '(';
        for (auto const& f : tpl.fields) {
            if (&f != &tpl.fields.front()) ss << ',';
            apply_visitor(*this, f);
        }
        ss << ')';
    }

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

small_string unit::as_string(identifier const& id) const
{
    boost::container::small_vector<char, 32> result;
    identifier_printer(id, ""sv, std::back_inserter(result), [](identifier const& id, string_view, auto &) {
        throw exception("identifier '%1%' has no string representation"_fmt % id.value);
    });
    return { result.data(), result.size() };
}

small_string unit::as_string(entity_identifier const& id) const
{
    THROW_NOT_IMPLEMENTED_ERROR("unit::as_string entity_identifier");
}

small_string unit::as_string(qname_view qn) const
{
    boost::container::small_vector<char, 32> result;
    name_printer(qn, std::back_inserter(result), [](identifier const& id, string_view, auto &) {
        throw exception("identifier '%1%' has no string representation"_fmt % id.value);
    });
    return { result.data(), result.size() };
}

small_string unit::as_string(qname_identifier name) const
{
    return as_string(functional_registry_.resolve(name).name());
}

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
    std::ostringstream& ss;
    explicit expr_printer_visitor(unit const& u, std::ostringstream& s) : u_{ u }, ss{ s } {}

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

    void operator()(context_value const& vi) const
    {

    }

    void operator()(variable_identifier const& vi) const
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

    void operator()(property_expression const& c) const
    {
        ss << "PROPERTY("sv << u_.print(c.name.value) << ")"sv;
    }

    void operator()(expression_vector_t const& ev) const
    {
        ss << '[';
        bool first = true;
        for (auto const& e : ev.elements) {
            if (!first) ss << ", "sv;
            else first = false;
            apply_visitor(*this, e);
        }
        ss << ']';
    }

    void operator()(bang_parameter_pack_t const& p) const
    {
        apply_visitor(*this, p.type);
        ss << "..."sv;
    }

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
        apply_visitor(*this, be[0].value());
        ss << ')';
    }

    void operator()(binary_expression_t const& be) const
    {
        ss << "binary("sv << (int)be.op << ", "sv;
        apply_visitor(*this, be[0].value());
        ss << ", "sv;
        apply_visitor(*this, be[1].value());
        ss << ')';
    }

    void operator()(case_expression const& f) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    void operator()(lambda const& f) const
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

    void operator()(opt_named_syntax_expression_list_t const& nel) const
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

    template <typename T>
    void operator()(T const& te) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    
};

std::string unit::print(syntax_expression_t const& e) const
{
    std::ostringstream ss;
    expr_printer_visitor vis{ *this, ss };
    apply_visitor(vis, e);
    return ss.str();
}

std::string unit::print(semantic::expression_t const& e) const
{
    std::ostringstream ss;
    semantic::expression_printer_visitor vis{ *this, ss };
    apply_visitor(vis, e);
    return ss.str();
}

std::string unit::print(small_u32string const& str, bool in_quotes) const
{
    namespace cvt = boost::conversion;
    std::string buff;
    if (in_quotes) buff.push_back('`');
    (cvt::cvt_push_iterator(cvt::utf32 | cvt::utf8, std::back_inserter(buff)) << str).flush();
    if (in_quotes) buff.push_back('`');
    return buff;
}

std::string unit::print(lex::resource_location const& loc) const
{
    return ("%1%(%2%,%3%)"_fmt % loc.resource % loc.line % loc.column).str();
}

std::string unit::print(error const& err) const
{
    std::ostringstream ss;
    error_printer_visitor vis{ *this, ss };
    error const* perr = &err;
    for (;;) {
        perr->visit(vis);
        auto & cause = perr->cause();
        if (!cause) break;
        perr = cause.get();
        ss << "\ncaused by: "sv;
    }
    return ss.str();
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

functional& unit::resolve_functional(qname_identifier fid)
{
    return fregistry().resolve(fid);
    //functional& f = fregistry().resolve(fid);
}

void unit::push_back_expression(semantic::expression_list_t& l, semantic::expression_t&& e)
{
    semantic::expression_list_t::entry_type * pentry = semantic_expression_list_entry_pool_.new_object(std::move(e));
    l.push_back(*pentry);
}

void unit::release(semantic::expression_list_t::entry_type&& e)
{
    semantic_expression_list_entry_pool_.delete_object(&e);
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

unit::unit()
    : slregistry_{ identifier_builder_ }
    , piregistry_{ identifier_builder_ }
    , fn_identifier_counter_ { (size_t)virtual_stack_machine::builtin_fn::eof_type }
    , bvm_{ std::make_unique<virtual_stack_machine>() }
    , semantic_expression_list_entry_pool_{ 128, 128 }
{
    //set_extern("string(const __id)"sv, &bang_print_string);

    //qname_identifier string_lid = make_qname_identifier("string");
    //functional& string_fnl = fregistry().resolve(string_lid);
    //functional::pattern p()
    //string_fnl.push();

    //identifier id_lid = slregistry().resolve("__id");

    // internal types
    //auto ie = make_shared<internal_type_entity>(make_qname_identifier("integer"));
    //integer_entity_ = ie.get();
    //eregistry_insert(std::move(ie));
#if 0
    auto de = make_shared<internal_type_entity>(make_qname_identifier("decimal"));
    decimal_entity_ = de.get();
    eregistry_insert(std::move(de));

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
    // typename
    auto typename_qname_identifier = make_qname_identifier("typename");
    auto typename_entity = make_shared<basic_signatured_entity>();
    typename_entity->set_signature(entity_signature{typename_qname_identifier});

    eregistry_insert(typename_entity);
    builtin_eids_[(size_t)builtin_eid::typename_] = typename_entity->id();
    typename_entity->set_type(get(builtin_eid::typename_));

    // any
    builtin_qnids_[(size_t)builtin_qnid::any] = make_qname_identifier("any");

    auto any_entity = make_shared<basic_signatured_entity>(get(builtin_eid::typename_), entity_signature{ get(builtin_qnid::any) });
    eregistry_insert(any_entity);
    builtin_eids_[(size_t)builtin_eid::any] = any_entity->id();
    functional& any_fnl = fregistry().resolve(get(builtin_qnid::any));
    BOOST_VERIFY(!any_fnl.set_default_entity(annotated_entity_identifier{ any_entity->id() }));

    // bool
    setup_type("bool"sv, builtin_qnids_[(size_t)builtin_qnid::boolean], builtin_eids_[(size_t)builtin_eid::boolean]);

    // integer
    setup_type("integer"sv, builtin_qnids_[(size_t)builtin_qnid::integer], builtin_eids_[(size_t)builtin_eid::integer]);

    // decimal
    setup_type("decimal"sv, builtin_qnids_[(size_t)builtin_qnid::decimal], builtin_eids_[(size_t)builtin_eid::decimal]);

    // string
    setup_type("string"sv, builtin_qnids_[(size_t)builtin_qnid::string], builtin_eids_[(size_t)builtin_eid::string]);

    // object
    setup_type("object"sv, builtin_qnids_[(size_t)builtin_qnid::object], builtin_eids_[(size_t)builtin_eid::object]);

    // identifier
    setup_type("__identifier"sv, builtin_qnids_[(size_t)builtin_qnid::identifier], builtin_eids_[(size_t)builtin_eid::identifier]);

    // identifier
    setup_type("__qname"sv, builtin_qnids_[(size_t)builtin_qnid::qname], builtin_eids_[(size_t)builtin_eid::qname]);

    // metaobject
    setup_type("metaobject"sv, builtin_qnids_[(size_t)builtin_qnid::metaobject], builtin_eids_[(size_t)builtin_eid::metaobject]);

    // tuple
    builtin_qnids_[(size_t)builtin_qnid::tuple] = make_qname_identifier("tuple");
    
    // void
    auto void_entity = make_shared<basic_signatured_entity>(get(builtin_eid::typename_), entity_signature{ get(builtin_qnid::tuple) });
    eregistry_insert(void_entity);
    builtin_eids_[(size_t)builtin_eid::void_] = void_entity->id();

    //// operations
    builtin_qnids_[(size_t)builtin_qnid::typeof] = make_qname_identifier("typeof");
    builtin_qnids_[(size_t)builtin_qnid::make_tuple] = make_qname_identifier("make_tuple");
    builtin_qnids_[(size_t)builtin_qnid::implicit_cast] = make_qname_identifier("implicit_cast");
    builtin_qnids_[(size_t)builtin_qnid::eq] = make_qname_identifier("equal");
    builtin_qnids_[(size_t)builtin_qnid::ne] = make_qname_identifier("not_equal");
    builtin_qnids_[(size_t)builtin_qnid::plus] = make_qname_identifier("__plus");
    builtin_qnids_[(size_t)builtin_qnid::negate] = make_qname_identifier("negate");
    builtin_qnids_[(size_t)builtin_qnid::get] = make_qname_identifier("get");
    builtin_qnids_[(size_t)builtin_qnid::set] = make_qname_identifier("set");

    /////// built in patterns
    // make_tuple(...) -> tuple(...)
    functional& make_tuple_fnl = fregistry().resolve(get(builtin_qnid::make_tuple));
    make_tuple_fnl.push(make_shared<make_tuple_pattern>());
    
    
    qname_identifier getfn = make_qname_identifier("get"sv);
    functional& get_fnl = fregistry().resolve(getfn);
    // get(self: tuple(), property: __identifier)->T;
    get_fnl.push(make_shared<tuple_get_pattern>());
    // get(self: @structure, property: __identifier)->T;
    get_fnl.push(make_shared<struct_get_pattern>());

    // __id(const string) -> __identifier
    qname_identifier idfn = make_qname_identifier("__id"sv);
    functional& idfnl = fregistry().resolve(idfn);
    idfnl.push(make_shared<create_identifier_pattern>());

    // metaobject(...) -> metaobject
    functional& metaobject_fnl = fregistry().resolve(get(builtin_qnid::metaobject));
    metaobject_fnl.push(make_shared<metaobject_pattern>());

    // typeof(object: const metaobject, property: const __identifier) -> typename
    functional& typeof_fnl = fregistry().resolve(get(builtin_qnid::typeof));
    typeof_fnl.push(make_shared<typeof_pattern>());

    // new(type: typename $T @struct, ...) -> $T
    builtin_qnids_[(size_t)builtin_qnid::new_] = make_qname_identifier("new"sv);
    functional& newfnl = fregistry().resolve(get(builtin_qnid::new_));
    newfnl.push(make_shared<new_struct_pattern>());

    // operator...(type: typename)
    builtin_qnids_[(size_t)builtin_qnid::ellipsis] = make_qname_identifier("..."sv);
    
    functional& ellipsis_fnl = fregistry().resolve(get(builtin_qnid::ellipsis));
    ellipsis_fnl.push(make_shared<ellipsis_pattern>());

    //// parameters
    builtin_ids_[(size_t)builtin_id::type] = make_identifier("__type"sv);
    builtin_ids_[(size_t)builtin_id::to] = make_identifier("to"sv);
    builtin_ids_[(size_t)builtin_id::self] = make_identifier("self"sv);
    builtin_ids_[(size_t)builtin_id::property] = make_identifier("property"sv);
    builtin_ids_[(size_t)builtin_id::object] = make_identifier("object"sv);
    builtin_ids_[(size_t)builtin_id::numargs] = make_identifier("$$"sv);

    //fn_result_identifier_ = make_identifier("->");

    //eq_qname_identifier_ = make_qname_identifier("==");
    //functional& eq_fnl = fregistry().resolve(eq_qname_identifier_);
    //eq_fnl.push(make_shared<eq_pattern>());

    builtin_qnids_[(size_t)builtin_qnid::fn] = make_qname_identifier("__fn"sv);
    
    builtin_eids_[(size_t)builtin_eid::arrayify] = set_builtin_extern("__arrayify"sv, &bang_arrayify);
    builtin_eids_[(size_t)builtin_eid::array_at] = set_builtin_extern("__array_at"sv, &bang_array_at);
    //set_extern<external_fn_pattern>("arrayify(...)->any"sv, &bang_arrayify);

    set_extern<external_fn_pattern>("print(any ...)"sv, &bang_print_string);
    //set_extern("implicit_cast(to: typename string, _)->string"sv, &bang_tostring);
    set_const_extern<to_string_pattern>("to_string(const __identifier)->string"sv);
    set_extern<external_fn_pattern>("to_string(_)->string"sv, &bang_tostring);
    set_extern<external_fn_pattern>("implicit_cast(to: typename decimal, integer)->decimal"sv, &bang_int2dec);
    set_extern<external_fn_pattern>("create_extern_object(string)->object"sv, &bang_create_extern_object);
    //set_extern<external_fn_pattern>("set(self: object, property: const __identifier, any)"sv, &bang_set_object_property);
    set_extern<external_fn_pattern>("set(self: object, property: string, any)->object"sv, &bang_set_object_property);

    //set_extern("string(any)->string"sv, &bang_tostring);
    set_extern<external_fn_pattern>("assert(bool)"sv, &bang_assert);

    // temporary
    set_extern<external_fn_pattern>("equal(any,any)->bool"sv, &bang_any_equal);
    set_extern<external_fn_pattern>("negate(any)->bool"sv, &bang_negate);
    set_extern<external_fn_pattern>("__plus(integer,integer)->integer"sv, &bang_operator_plus_integer);
    set_extern<external_fn_pattern>("__plus(decimal,decimal)->decimal"sv, &bang_operator_plus_decimal);
}

}

