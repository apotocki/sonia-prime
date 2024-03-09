//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <fstream>

#include "sonia/filesystem.hpp"
#include "sonia/exceptions.hpp"

#include "sonia/utility/lang/utility.hpp"
#include "sonia/utility/lang/identifier_builder.hpp"
#include "sonia/utility/lang/string_literal_registry.hpp"
#include "sonia/utility/lang/parameterized_identifier_registry.hpp"
#include "sonia/utility/lang/entity_registry.hpp"

#include "boost/conversion/unicode/utf.hpp"
#include "boost/conversion/push_iterator.hpp"

#include "beng.hpp"

//#include "ast.hpp"
#include "semantic.hpp"
#include "variable_entity.hpp"

//#include "functional_entity.hpp"

namespace sonia::lang::beng {

/*
struct variable_descriptor
{
    identifier name;
    beng_generic_type type;
    uint32_t index : 31;
    uint32_t is_const : 1;

    explicit variable_descriptor(size_t idx) : index { static_cast<uint32_t>(idx) } {}

    friend inline bool operator==(variable_descriptor const& l, variable_descriptor const& r) noexcept { return l.name == r.name; }
    friend inline auto operator<=>(variable_descriptor const& l, variable_descriptor const& r) noexcept { return l.name <=> r.name; }
    friend inline size_t hash_value(variable_descriptor const& v) noexcept { return hash_value(v.name); }
};

struct variable_equal_to
{
    inline bool operator()(variable_descriptor const& l, identifier const& r) const noexcept { return l.name == r; }
    inline bool operator()(identifier const& l, variable_descriptor const& r) const noexcept { return l == r.name; }
};
*/

class unit
{
    using identifier_builder_t = identifier_builder<identifier>;
    using slregistry_t = string_literal_registry<identifier, small_string>;
    using piregistry_t = parameterized_identifier_registry<identifier>;
    using eregistry_t = entity_registry<entity>;

    identifier_builder_t identifier_builder_;
    slregistry_t slregistry_;
    piregistry_t piregistry_;
    eregistry_t eregistry_;

    // semantic
    
    //std::vector<shared_ptr<function_t>> functions_;
    //shared_ptr<function_t> main_function_;
    std::vector<semantic_expression_type> root_expressions_;

    shared_ptr<entity> arrayify_entity_;
    shared_ptr<entity> print_entity_;

    qname arrayify_entity_name_;
    qname print_string_name_;

public:
    unit()
        : slregistry_{ identifier_builder_ }
        , piregistry_{ identifier_builder_ }
    {
        // install builtin entities
        arrayify_entity_name_ = qname{ new_identifier() };
        //arrayify_entity_ = make_shared<entity>(qname{ new_identifier() });
        //eregistry_.insert(arrayify_entity_);


        // print (string)
        qname print_qn{ slregistry_.resolve("print"sv) };
        qname print_args{ slregistry_.resolve("string"sv) };
        identifier particularprintid = piregistry_.resolve(span{ &print_args, 1 });
        print_string_name_ = print_qn + particularprintid;

        //print_entity_ = make_shared<entity>(qname{ slregistry_.resolve("print"sv) });
        //eregistry_.insert(print_entity_);
    }

    identifier new_identifier() { return identifier_builder_(); }

    slregistry_t& slregistry() { return slregistry_; }
    piregistry_t& piregistry() { return piregistry_; }
    eregistry_t& eregistry() { return eregistry_; }

    //void put_function(shared_ptr<function_t> f)
    //{
    //    functions_.emplace_back(std::move(f));
    //}

    ////////////////////// builtins
    qname const& arrayify_entity_name() const { return arrayify_entity_name_; }
    qname const& print_string_name() const { return print_string_name_; }

    ///
    void append_path(fs::path p)
    {
        additional_paths_.emplace_back(std::move(p));
    }

    std::vector<char> get_file_content(fs::path const& rpath, fs::path const* context = nullptr)
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
    
    std::string print(identifier const& id) const
    {
        std::ostringstream ss;
        if (auto const* pstr = slregistry_.resolve(id); pstr) {
            ss << *pstr;
        } else {
            ss << "$"sv << id.value;
        }
        return ss.str();
    }

    std::string print(qname const& q) const
    {
        return print((qname_view)q);
    }

    std::string print(qname_view q) const
    {
        std::ostringstream ss;
        for (identifier const& id : q) {
            if (&q.front() != &id || q.is_absolute()) {
                ss << "::"sv;
            }
            if (auto const* pstr = slregistry_.resolve(id); pstr) {
                ss << *pstr;
            } else if (auto sp = piregistry_.resolve(id); !sp.empty()) {
                ss << '<';
                for (auto const& qn : sp) {
                    if (&qn != &sp.front()) ss << ',';
                    ss << print(qn);
                }
                ss << '>';
            } else {
                ss << "$"sv << id.value;
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
        inline void operator()(beng_object_t const& obj) const { ss << '^' << u_.print(obj.name()); }
        inline void operator()(beng_fn_t const& fn) const
        {
            apply_visitor(*this, fn.arg);
            ss << "->";
            apply_visitor(*this, fn.result);
        }
        inline void operator()(beng_vector_t const& v) const
        {
            ss << '[';
            apply_visitor(*this, v.type);
            ss << ']';
        }
        inline void operator()(beng_array_t const& arr) const
        {
            apply_visitor(*this, arr.type);
            ss << '[' << arr.size << ']';
        }
        inline void operator()(beng_tuple_t const& tpl) const
        {
            ss << '(';
            for (auto const& f : tpl.fields) {
                if (&f != &tpl.fields.front()) ss << ',';
                apply_visitor(*this, f);
            }
            ss << ')';
        }
        inline void operator()(beng_union_t const& tpl) const
        {
            for (auto const& f : tpl.members) {
                if (&f != &tpl.members.front()) ss << "||";
                apply_visitor(*this, f);
            }
        }
    };

    std::string print(beng_generic_type const& tp) const
    {
        std::ostringstream ss;
        type_printer_visitor vis{ *this, ss };
        apply_visitor(vis, tp);
        return ss.str();
    }

    string_view as_string(identifier const& id) const
    {
        if (auto const* pstr = slregistry_.resolve(id); pstr) {
            return *pstr;
        }
        throw exception("identifier '%1%' has no string representation"_fmt % id.value);
    }

    small_u32string as_u32string(identifier const& id) const
    {
        namespace cvt = boost::conversion;
        if (auto const* pstr = slregistry_.resolve(id); pstr) {
            boost::container::small_vector<char32_t, 32> result;
            (cvt::cvt_push_iterator(cvt::utf8 | cvt::utf32, std::back_inserter(result)) << *pstr).flush();
            return small_u32string{ result.data(), result.size() };
        }
        throw exception("identifier '%1%' has no string representation"_fmt % id.value);
    }

    small_u32string as_u32string(qname_view name) const
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

    struct type_mangler_visitor : static_visitor<qname>
    {
        unit & u_;
        explicit type_mangler_visitor(unit & u) : u_{ u } {}

        inline result_type operator()(beng_bool_t) const { return qname{ u_.slregistry().resolve("bool"sv) }; }
        inline result_type operator()(beng_int_t) const { return qname{ u_.slregistry().resolve("int"sv)}; }
        inline result_type operator()(beng_float_t) const { return qname{ u_.slregistry().resolve("float"sv) }; }
        inline result_type operator()(beng_decimal_t) const { return qname{ u_.slregistry().resolve("decimal"sv) }; }
        inline result_type operator()(beng_string_t) const { return qname{ u_.slregistry().resolve("string"sv) }; }
        inline result_type operator()(beng_object_t const& obj) const { return qname{ obj.name(), true }; }

        inline result_type operator()(beng_fn_t const& fn) const
        {
            THROW_NOT_IMPLEMENTED_ERROR();
            /*
            apply_visitor(*this, fn.arg);
            ss << "->";
            apply_visitor(*this, fn.result);
            */
        }
        inline result_type operator()(beng_vector_t const& v) const
        {
            THROW_NOT_IMPLEMENTED_ERROR();
            /*
            ss << '[';
            apply_visitor(*this, v.type);
            ss << ']';
            */
        }
        inline result_type operator()(beng_array_t const& arr) const
        {
            THROW_NOT_IMPLEMENTED_ERROR();
            /*
            apply_visitor(*this, arr.type);
            ss << '[' << arr.size << ']';
            */
        }
        inline result_type operator()(beng_tuple_t const& tpl) const
        {
            THROW_NOT_IMPLEMENTED_ERROR();
            /*
            ss << '(';
            for (auto const& f : tpl.fields) {
                if (&f != &tpl.fields.front()) ss << ',';
                apply_visitor(*this, f);
            }
            ss << ')';
            */
        }
        inline result_type operator()(beng_union_t const& tpl) const
        {
            THROW_NOT_IMPLEMENTED_ERROR();
            /*
            for (auto const& f : tpl.members) {
                if (&f != &tpl.members.front()) ss << "||";
                apply_visitor(*this, f);
            }
            */
        }
    };

    void build_name(qname_view base, function_signature& sig)
    {
        type_mangler_visitor vis{ *this };

        std::vector<qname> ps;

        for (beng_generic_type const& postp: sig.position_parameters) {
            ps.emplace_back(apply_visitor(vis, postp));
        }
        for (auto const& [id, tp] : sig.named_parameters) {
            ps.emplace_back(qname{ id });
            ps.emplace_back(apply_visitor(vis, tp));
        }
        identifier id = piregistry_.resolve(ps);
        sig.mangled_name = base.empty() ? qname{ id } : (base + id);
    }

protected:
    std::vector<char> read_file(fs::path const& rpath)
    {
        std::vector<char> result;
        result.reserve(fs::file_size(rpath));
        std::ifstream file{ rpath.string().c_str(), std::ios::binary };
        std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{}, std::back_inserter(result));
        return result;
    }

private:
    // entities registry:
    //qname -> entity

    std::vector<fs::path> additional_paths_;
};

}
