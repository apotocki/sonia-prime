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
#include "entities/functions/external_fn_pattern.hpp"
#include "entities/compare/compare_patterns.hpp"

#include "semantic/expression_printer.hpp"

namespace sonia::lang::bang {

functional& unit::resolve_functional(qname_view qn)
{
    assert(qn.is_absolute());
    return fregistry().resolve(qn);
}

qname_identifier unit::get_function_entity_identifier(string_view signature)
{
    parser_context parser{ *this };
    auto decls = parser.parse((string_view)("extern fn %1%;"_fmt % signature).str());

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
//    eregistry().insert(std::move(e));
//}

//size_t unit::allocate_constant_index()
//{
//    return bvm().add_const(smart_blob{});
//}

std::string unit::describe_efn(size_t fn_index) const
{
    auto pair = bvm_->efns().at(fn_index);
    if (std::get<1>(pair).empty()) {
        return ("#%1% at 0x%2$x"_fmt % fn_index % (uintptr_t)std::get<0>(pair)).str();
    } else {
        return ("#%1% '%2%'"_fmt % fn_index % std::string(std::get<1>(pair))).str();
    }
}

void unit::set_extern(string_view signature, void(*pfn)(vm::context&))
{
    parser_context parser{ *this };
    auto decls = parser.parse((string_view)("extern fn ::%1%;"_fmt % signature).str());
    if (!decls.has_value()) [[unlikely]] {
        throw exception(decls.error());
    }
    fn_compiler_context ctx{ *this, qname{} };
    auto & fndecl = get<fn_pure_t>(decls->front());

    // if the result is not defined we can not resove it (e.g. from the function body) => suppose that it is void
    if (!fndecl.result) {
        fndecl.result = entity_expression(get_void_entity_identifier(), fndecl.location());
    }

    auto eptrn = make_shared<external_fn_pattern>(ctx, fndecl, fn_identifier_counter_);
    functional& f = ctx.u().fregistry().resolve(eptrn->fn_qname_id());
    f.push(eptrn);

    // to do: mangled name
    bvm_->set_efn(fn_identifier_counter_++, pfn, small_string{print(fndecl.name())});

    //function_signature sig;
    //sig.setup(ctx, fndecl.parameters);
    //sig.normilize(ctx);

    //symbol smb{ fd.id() };
    //sig.build_symbol(*this, smb);

//    THROW_NOT_IMPLEMENTED_ERROR("unit::set_extern");
#if 0
    declaration_visitor dvis{ ctx };
    auto & fsig = dvis.append_fnsig(fndecl);

    qname fnm = fndecl.name() / fsig.mangled_id;



    auto pefe = make_shared<external_function_entity>(qnregistry().resolve(fnm), fn_identifier_counter_);
    eregistry_.insert(pefe);

    strings_.emplace_back(print(fnm));
    bvm_->set_efn(fn_identifier_counter_++, pfn, strings_.back());
#endif
}

unit::unit()
    : slregistry_{ identifier_builder_ }
    , piregistry_{ identifier_builder_ }
    , fn_identifier_counter_ { (size_t)virtual_stack_machine::builtin_fn::eof_type }
    , bvm_{ std::make_unique<virtual_stack_machine>() }
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
    //eregistry().insert(std::move(ie));
#if 0
    auto de = make_shared<internal_type_entity>(make_qname_identifier("decimal"));
    decimal_entity_ = de.get();
    eregistry().insert(std::move(de));

    // internal functions
    builtins_.resize((size_t)builtin_fn::eof_builtin_type);


    auto parrayify = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::arrayify);
    eregistry_.insert(parrayify);
    set_efn(builtin_fn::arrayify, parrayify->name());

    auto punpack = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::unpack);
    eregistry_.insert(punpack);
    set_efn(builtin_fn::unpack, punpack->name());

    auto pweak_create = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::weak_create);
    eregistry_.insert(pweak_create);
    set_efn(builtin_fn::weak_create, pweak_create->name());

    auto pweak_lock = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::weak_lock);
    eregistry_.insert(pweak_lock);
    set_efn(builtin_fn::weak_lock, pweak_lock->name());

    auto pce = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::extern_object_create);
    eregistry_.insert(pce);
    set_efn(builtin_fn::extern_object_create, pce->name());

    auto peosp = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::extern_object_set_property);
    eregistry_.insert(peosp);
    set_efn(builtin_fn::extern_object_set_property, peosp->name());

    auto peogp = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::extern_object_get_property);
    eregistry_.insert(peogp);
    set_efn(builtin_fn::extern_object_get_property, peogp->name());

    auto pefc = make_shared<external_function_entity>(new_qname_identifier(), (size_t)virtual_stack_machine::builtin_fn::extern_function_call);
    eregistry_.insert(pefc);
    set_efn(builtin_fn::extern_function_call, pefc->name());

    auto ptostring = make_shared<external_function_entity>(new_qname_identifier(), fn_identifier_counter_);
    eregistry_.insert(ptostring);
    strings_.emplace_back("tostring");
    set_efn(builtin_fn::tostring, ptostring->name());
    bvm_->set_efn(fn_identifier_counter_++, &bang_tostring, strings_.back());

    auto pnegate = make_shared<external_function_entity>(new_qname_identifier(), fn_identifier_counter_);
    eregistry_.insert(pnegate);
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

    eregistry().insert(typename_entity);
    typename_entity_identifier_ = typename_entity->id();
    typename_entity->set_type(typename_entity_identifier_);

    // any
    any_qname_identifier_ = make_qname_identifier("any");
    
    auto any_entity = make_shared<basic_signatured_entity>(typename_entity_identifier_, entity_signature{ any_qname_identifier_ });
    eregistry().insert(any_entity);
    any_entity_identifier_ = any_entity->id();
    functional& any_fnl = fregistry().resolve(any_qname_identifier_);
    any_fnl.set_default_entity(any_entity->id());
    
    // bool
    setup_type("bool"sv, bool_qname_identifier_, bool_entity_identifier_);
    
    // string
    setup_type("string"sv, string_qname_identifier_, string_entity_identifier_);

    // decimal
    setup_type("decimal"sv, decimal_qname_identifier_, decimal_entity_identifier_);

    // tuple
    qname_identifier tuple_qnid = make_qname_identifier("tuple"sv);
    
    // void
    auto void_entity = make_shared<basic_signatured_entity>(typename_entity_identifier_, entity_signature{ tuple_qnid });
    eregistry().insert(void_entity);

    void_entity_identifier_ = void_entity->id();

    //// operations
    // eq
    eq_qname_identifier_ = make_qname_identifier("equal"sv);
    ne_qname_identifier_ = make_qname_identifier("not_equal"sv);
    negate_qname_identifier_ = make_qname_identifier("negate"sv);

    //eq_qname_identifier_ = make_qname_identifier("==");
    //functional& eq_fnl = fregistry().resolve(eq_qname_identifier_);
    //eq_fnl.push(make_shared<eq_pattern>());

    fn_qname_identifier_ = make_qname_identifier("__fn"sv);
    fn_result_identifier_ = make_identifier("->");
    // setup ellipsis
    // operator...(type: typename)
    ellipsis_qname_identifier_ = make_qname_identifier("...");
    //function_descriptor fn_ellipsis{ ellipsis_qname_identifier_ };
    //fn_ellipsis.push_field(nullptr, typename_entity->id(), true);

    functional& ellipsis_fnl = fregistry().resolve(ellipsis_qname_identifier_);
    // no default entity
    // ellipsis_fnl.set_default_entity({});
    ellipsis_fnl.push(make_shared<ellipsis_pattern>());
    
    //functional::pattern p()
    //string_fnl.push();

    set_extern("print(string...)"sv, &bang_print_string);
    set_extern("string(any)->string"sv, &bang_tostring);
    set_extern("assert(bool)"sv, &bang_assert);

    // temporary
    set_extern("equal(decimal,decimal)->bool"sv, &bang_decimal_equal);
    set_extern("negate(any)->bool"sv, &bang_negate);
}

void unit::setup_type(string_view type_name, qname_identifier& qnid, entity_identifier& eid)
{
    qnid = make_qname_identifier(type_name);
    functional& some_type_fnl = fregistry().resolve(qnid);
    auto some_type_entity = make_shared<basic_signatured_entity>(typename_entity_identifier_, entity_signature{ qnid });
    eregistry().insert(some_type_entity);
    some_type_fnl.set_default_entity(some_type_entity->id());
    eid = some_type_entity->id();
}

void unit::set_efn(size_t idx, qname_identifier fnq)
{
    if (builtins_.size() <= idx) {
        builtins_.resize(idx + 1);
    }
    builtins_[idx] = std::move(fnq);
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
    for (identifier const& id : qn) {
        oi = identifier_printer(id, &qn.front() == &id /* && !qn.is_absolute()*/ ? ""sv : "::"sv, std::move(oi), uf);
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
    eregistry_.get(id).print_to(ss, *this);
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
        for (auto const&f : sgn.positioned_fields()) {
            if (first) first = false;
            else ss << ", "sv;
            if (f.is_const()) ss << "const "sv;
            ss << print(f.entity_id());
        }
        for (auto const& f : sgn.named_fields()) {
            if (first) first = false;
            else ss << ", "sv;
            ss << print(f.first) << ": "sv;
            if (f.second.is_const()) ss << "const "sv;
            ss << print(f.second.entity_id());
        }
        ss << ")";
    }
    return ss.str();
}

std::string unit::print(qname_view qn) const
{
    boost::container::small_vector<char, 32> result;
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

    inline void operator()(bang_preliminary_tuple_t const& tpl) const
    {
        ss << '(';
        for (auto const& f : tpl.fields) {
            if (&f != &tpl.fields.front()) ss << ',';
            if (f.name) {
                ss << u_.print(f.name->value) << ": "sv;
            }
            apply_visitor(*this, f.type);
        }
        ss << ')';
    }

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

    inline void operator()(bang_bunion_t const& bu) const
    {
        ss << "{true: "sv;
        apply_visitor(*this, bu.true_type);
        ss << ", false: "sv;
        apply_visitor(*this, bu.true_type);
        ss << '}';
    }

    inline void operator()(bang_union_t const& tpl) const
    {
        bool first = true;
        for (auto const& f : tpl) {
            if (!first) ss << "|"sv; else first = false;
            apply_visitor(*this, f);
        }
    }

    template <typename FamilyT>
    inline void operator()(bang_preliminary_union<FamilyT> const& tpl) const
    {
        for (auto const& f : tpl.members) {
            if (&f != &tpl.members.front()) ss << "||";
            apply_visitor(*this, f);
        }
    }
};

std::string unit::print(bang_preliminary_type const& tp) const
{
    std::ostringstream ss;
    type_printer_visitor vis{ *this, ss };
    apply_visitor(vis, tp);
    return ss.str();
}

std::string unit::print(bang_type const& tp) const
{
    std::ostringstream ss;
    type_printer_visitor vis{ *this, ss };
    apply_visitor(vis, tp);
    return ss.str();
}

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
        if (vi.implicit) {
            ss << "IMPLICIT"sv;
        }
        ss << "VAR("sv << u_.print(vi.name.value) << ")"sv;
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
        ss << ", "sv << u_.print(c.name.value) << ")"sv;
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
        apply_visitor(*this, be.positioned_args[0]);
        ss << ')';
    }

    void operator()(binary_expression_t const& be) const
    {
        ss << "binary("sv << (int)be.op << ", "sv;
        apply_visitor(*this, be.positioned_args[0]);
        ss << ", "sv;
        apply_visitor(*this, be.positioned_args[1]);
        ss << ')';
    }

    void operator()(case_expression const& f) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    void operator()(lambda_t const& f) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    void operator()(entity_expression const& f) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    /*
    template <typename T>
    void operator()(T const& te) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    */
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
    err.visit(vis);
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

}
