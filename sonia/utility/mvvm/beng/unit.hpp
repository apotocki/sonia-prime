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

public:
    unit()
        : slregistry_{ identifier_builder_ }
        , piregistry_{ identifier_builder_ }
    {}

    slregistry_t& slregistry() { return slregistry_; }
    piregistry_t& piregistry() { return piregistry_; }
    eregistry_t& eregistry() { return eregistry_; }

    //void put_function(shared_ptr<function_t> f)
    //{
    //    functions_.emplace_back(std::move(f));
    //}

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

    std::string print(qname_view q) const
    {
        std::ostringstream ss;
        for (identifier const& id : q) {
            if (&q.front() != &id || q.is_absolute()) {
                ss << "::"sv;
            }
            if (auto const* pstr = slregistry_.resolve(id); pstr) {
                ss << *pstr;
            } else {
                ss << "$"sv << id.value;
            }
        }
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
