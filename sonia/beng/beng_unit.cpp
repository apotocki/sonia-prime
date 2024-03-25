//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "unit.hpp"
#include "library/library.hpp"

#include "parser.hpp"
#include "ast/fn_compiler_context.hpp"
#include "ast/declaration_visitor.hpp"

namespace sonia::lang::beng {

void unit::set_extern(string_view signature, void(*pfn)(vm::context&))
{
    parser_context parser{ *this };
    parser.parse((string_view)("extern fn ::%1%;"_fmt % signature).str());
    
    fn_compiler_context ctx{ *this, qname{} };
    auto & fndecl = get<fn_pure_decl>(parser.generic_declarations().front());

    declaration_visitor dvis{ ctx };
    auto & fsig = dvis.append_fnsig(fndecl);

    qname fnm = fndecl.name() + fsig.mangled_id;
    auto pefe = make_shared<external_function_entity>(fnm, fn_identifier_counter_);
    eregistry_.insert(pefe);

    strings_.emplace_back(print(fnm));
    bvm_.set_efn(fn_identifier_counter_++, pfn, strings_.back());
}

unit::unit()
    : slregistry_{ identifier_builder_ }
    , piregistry_{ identifier_builder_ }
    , fn_identifier_counter_ { (size_t)virtual_stack_machine::builtin_fn::eof_type }
{
    builtins_.resize((size_t)builtin_fn::eof_builtin_type);

    auto parrayify = make_shared<external_function_entity>(qname{ new_identifier() }, (size_t)virtual_stack_machine::builtin_fn::arrayify);
    eregistry_.insert(parrayify);
    set_efn(builtin_fn::arrayify, parrayify->name());

    auto ptostring = make_shared<external_function_entity>(qname{ new_identifier() }, fn_identifier_counter_);
    eregistry_.insert(ptostring);
    strings_.emplace_back("tostring");
    set_efn(builtin_fn::tostring, ptostring->name());
    bvm_.set_efn(fn_identifier_counter_++, &beng_tostring, strings_.back());

    set_extern("print(string)"sv, &beng_print_string);

    /*
    // install builtin entities
    

    //arrayify_entity_ = make_shared<entity>(qname{ new_identifier() });
    //eregistry_.insert(arrayify_entity_);


    // print (string)
    qname print_qn{ slregistry_.resolve("print"sv) };
    qname print_args{ slregistry_.resolve("string"sv) };
    identifier particularprintid = piregistry_.resolve(span{ &print_args, 1 });
    builtins_[(int)builtin_type::print_string] = print_qn + particularprintid;

    //print_entity_ = make_shared<entity>(qname{ slregistry_.resolve("print"sv) });
    //eregistry_.insert(print_entity_);
    */
}

void unit::set_efn(size_t idx, qname_view fnq)
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
            throw exception("can't resolve path: '%1%'"_fmt % rpath);
        }
        return read_file(rpath);
    }
            
    auto tested_path = context ? *context / rpath : rpath;
    for (auto it = additional_paths_.begin();;) {
        if (fs::exists(tested_path) && fs::is_regular_file(tested_path)) {
            return read_file(tested_path);
        }
        if (it == additional_paths_.end()) {
            throw exception("can't resolve path: '%1%'"_fmt % rpath);
        }
        tested_path = (*it++) / rpath;
    }
}

std::string unit::print(identifier const& id) const
{
    std::ostringstream ss;
    if (auto const* pstr = slregistry_.resolve(id); pstr) {
        ss << *pstr;
    } else {
        ss << "$"sv << id.value;
    }
    return ss.str();
}

std::string unit::print(qname_view q) const
{
    std::ostringstream ss;
    for (identifier const& id : q) {
        if (&q.front() != &id || q.is_absolute()) {
            ss << "::"sv;
        }
        if (auto const* pstr = slregistry_.resolve(id); pstr) {
            ss << *pstr;
        } else if (auto sp = piregistry_.resolve(id); sp) {
            ss << '<';
            for (auto const& qn : *sp) {
                if (&qn != &sp->front()) ss << ',';
                ss << print(qn);
            }
            ss << '>';
        } else {
            ss << "@"sv << id.value;
        }
    }
    return ss.str();
}

struct type_printer_visitor : static_visitor<void>
{
    unit const& u_;
    std::ostringstream & ss;
    explicit type_printer_visitor(unit const& u, std::ostringstream& s) : u_{u}, ss{s} {}

    inline void operator()(beng_bool_t) const { ss << "bool"; }
    inline void operator()(beng_int_t) const { ss << "int"; }
    inline void operator()(beng_float_t) const { ss << "float"; }
    inline void operator()(beng_decimal_t) const { ss << "decimal"; }
    inline void operator()(beng_string_t) const { ss << "string"; }
    inline void operator()(beng_preliminary_object_t const& obj) const { ss << '^' << u_.print(obj.name()); }
    inline void operator()(beng_object_t const& obj) const { ss << '^' << u_.print(obj.name()); }
        
    template <typename TupleT, typename FamilyT>
    inline void operator()(beng_fn_base<TupleT, FamilyT> const& fn) const
    {
        (*this)(fn.arg);
        ss << "->";
        apply_visitor(*this, fn.result);
    }

    template <typename FamilyT>
    inline void operator()(beng_vector<FamilyT> const& v) const
    {
        ss << '[';
        apply_visitor(*this, v.type);
        ss << ']';
    }

    template <typename FamilyT>
    inline void operator()(beng_array<FamilyT> const& arr) const
    {
        apply_visitor(*this, arr.type);
        ss << '[' << arr.size << ']';
    }

    inline void operator()(beng_preliminary_tuple_t const& tpl) const
    {
        ss << '(';
        for (auto const& f : tpl.fields) {
            if (&f != &tpl.fields.front()) ss << ',';
            if (f.name) {
                ss << u_.print(f.name->id) << ": "sv;
            }
            apply_visitor(*this, f.type);
        }
        ss << ')';
    }

    template <typename FamilyT>
    inline void operator()(beng_tuple<FamilyT> const& tpl) const
    {
        ss << '(';
        for (auto const& f : tpl.fields) {
            if (&f != &tpl.fields.front()) ss << ',';
            apply_visitor(*this, f);
        }
        ss << ')';
    }

    template <typename FamilyT>
    inline void operator()(beng_union<FamilyT> const& tpl) const
    {
        for (auto const& f : tpl.members) {
            if (&f != &tpl.members.front()) ss << "||";
            apply_visitor(*this, f);
        }
    }
};

std::string unit::print(beng_preliminary_type const& tp) const
{
    std::ostringstream ss;
    type_printer_visitor vis{ *this, ss };
    apply_visitor(vis, tp);
    return ss.str();
}

std::string unit::print(beng_type const& tp) const
{
    std::ostringstream ss;
    type_printer_visitor vis{ *this, ss };
    apply_visitor(vis, tp);
    return ss.str();
}

string_view unit::as_string(identifier const& id) const
{
    if (auto const* pstr = slregistry_.resolve(id); pstr) {
        return *pstr;
    }
    throw exception("identifier '%1%' has no string representation"_fmt % id.value);
}

small_u32string unit::as_u32string(identifier const& id) const
{
    namespace cvt = boost::conversion;
    if (auto const* pstr = slregistry_.resolve(id); pstr) {
        boost::container::small_vector<char32_t, 32> result;
        (cvt::cvt_push_iterator(cvt::utf8 | cvt::utf32, std::back_inserter(result)) << *pstr).flush();
        return small_u32string{ result.data(), result.size() };
    }
    throw exception("identifier '%1%' has no string representation"_fmt % id.value);
}

small_u32string unit::as_u32string(qname_view name) const
{
    namespace cvt = boost::conversion;
    boost::container::small_vector<char32_t, 32> result;
    for (identifier const& id : name) {
        if (&name.front() != &id || name.is_absolute()) {
            result.push_back(':');
            result.push_back(':');
        }
        if (auto const* pstr = slregistry_.resolve(id); pstr) {
            (cvt::cvt_push_iterator(cvt::utf8 | cvt::utf32, std::back_inserter(result)) << *pstr).flush();
        } else {
            throw exception("identifier '%1%' has no string representation"_fmt % id.value);
            //result.push_back('$');
            //(cvt::cvt_push_iterator(cvt::utf8 | cvt::utf32, std::back_inserter(result)) << boost::lexical_cast<std::string>(id.value)).flush();
        }
    }
    return small_u32string{ result.data(), result.size() };
}

std::vector<char> unit::read_file(fs::path const& rpath)
{
    std::vector<char> result;
    result.reserve(fs::file_size(rpath));
    std::ifstream file{ rpath.string().c_str(), std::ios::binary };
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{}, std::back_inserter(result));
    return result;
}

}